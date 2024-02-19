#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <utime.h>
#include <assert.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/param.h>
#include <fs/f3s_spec.h>
#include <stack>
#include <limits>
#include <cstddef>
#include <map>
#include <sstream>

#include "utils.h"
#include "CFile.hpp"


std::string map_to_string(const std::map<f3s_extptr_t,CPath>& m){
    std::stringstream ss;
    ss << "{";
    for(const auto& pair :m){
        ss << fmt::format("{{ {0:s} : \"{1:s}\" }},", pair.first.to_str(), pair.second.c_str()); 
    }
    ss << "}";
    return ss.str();
}

template<size_t ecc_size>
struct f3s_unit_logi_ecc{
    _Uint16t     struct_size;  /* size of this structure */
    _Uint16t     logi;         /* logical unit number */
    _Uint32t     age;          /* age of logical unit */
    _Uint8t      ecc[ecc_size];
    _Uint32t     md5[4];       /* md5 checksum */
    
    f3s_unit_logi_t to_unit_logi()const{
        return f3s_unit_logi_t{
            .struct_size=sizeof(f3s_unit_logi_t), 
            .logi=logi, .age=age, 
            .md5 = {md5[0], md5[1], md5[2], md5[3]}, 
            };
    }
};

uint32_t BlockSize = (uint32_t)(-1);
int BlockIndex = 0;
char Endian='L';
uint32_t g_Alignment=4;
CPath g_InputEFS_FilePath;
CPath g_OutputDir;

struct HeadRevReaderException :public std::exception{
    std::string _msg;
    HeadRevReaderException(const std::string& msg)
        :_msg(msg)
    {
    }
};
struct SeekNextBlockEx :public std::exception{
};

struct HeadRevReader{
    CFile _fin;
    HeadRevReader(){
    }

    f3s_head_t Init(){
        this->_fin.open(g_InputEFS_FilePath, "rb");
        long file_pos = BlockSize*(BlockIndex+1) - sizeof(f3s_head_t);
        assert(is_power_of_2(BlockSize));

        this->_fin.seek(file_pos, SEEK_SET);
        f3s_head_t head={0};
        this->_fin >> head;

        if(!f3s_head_check(&head)){
            auto error_msg = fmt::format("HeadRevReader::init::f3s_head_check() failed");
            spdlog::error(error_msg);
            throw  HeadRevReaderException(error_msg);
        }
        if(head.text_size != sizeof(f3s_unit_info_t)){
            auto error_msg = fmt::format("head.text_size(={:04x}) != sizeof(f3s_unit_info_t)(={:04x})", head.text_size, sizeof(f3s_unit_info_t));
            spdlog::error(error_msg);
            throw  HeadRevReaderException(error_msg);
        }
        this->_fin.seek(file_pos, SEEK_SET);
        return head;
    }

    void updatePos(void){
        long file_pos = BlockSize*(BlockIndex+1) - sizeof(f3s_head_t);
        auto statB = g_InputEFS_FilePath.stat();
        if(file_pos >= statB.st_size ){
            throw CFileEofEx();
        }
        this->_fin.seek(file_pos, SEEK_SET);
    }

    f3s_head_t read(void){
        long file_pos = this->_fin.tell();
        spdlog::info("HeadRevReader::read::ftell={:08x}", file_pos);

        f3s_head_t head={0};
        this->_fin >> head;
        if(!f3s_head_check(&head)){
            auto error_msg = fmt::format("f3s_head_check(), file_pos={:08x}", file_pos);
            spdlog::info(error_msg);
            throw SeekNextBlockEx();
        }
        file_pos -= sizeof(f3s_head_t);
        this->_fin.seek(file_pos, SEEK_SET);
        spdlog::info("HeadRevReader::read::ftell={:08x}", this->_fin.tell());
        return head;
    }
};


HeadRevReader g_HeadRevReader;


inline void f3s_head_print(const f3s_head_t& head, long text_offset){
    auto status_str = head.status[0].to_str();
    printf(".status=%08x (%s)\n", 
        head.status[0].to_value(), status_str.data());
    printf(".text_offset=%08lX\n", text_offset);
    printf(".text_size=%04X\n", head.text_size);        
}


void TouchFile(const CPath& filePath, const f3s_stat_t& st){
    {
        CFile fout(filePath, "wb");
    }
    
    long res = 0;
    ::mode_t fmode = st.mode.to_value() & 00777;
    res = ::chmod(filePath.c_str(), fmode);
    if(res!=0){
        res = errno;
        auto err_msg = fmt::format("chmod(\"{}\", \"0o{:08o}\") errno={}", filePath, fmode, res);
        spdlog::error(err_msg);
    }
    struct utimbuf utimeParams={.actime=st.mtime, .modtime=st.mtime};
    res = ::utime(filePath.c_str(), &utimeParams);
    if(res!=0){
        res = errno;
        auto err_msg=fmt::format("utime(\"{}\", atime={:08x},mtime={:08x}) errno={}", filePath, utimeParams.actime, utimeParams.modtime, res);
        spdlog::error(err_msg);
    }
}

void AppendFileData(const CPath& filePath, const void* inData, size_t dataLen){
    if(dataLen == 0){
        return;
    }
    assert(inData != NULL);
    CFile fout(filePath, "ab");
    fout.write(inData, dataLen);
}

bool CheckFilePosIntersected(CFile& ForwardFin){
    long forward_pos=0, backward_pos=0;
    CFile& BackwardFin = g_HeadRevReader._fin;
    forward_pos = ForwardFin.tell();
    backward_pos = BackwardFin.tell();
    if(forward_pos < backward_pos){
        return false;
    }else{
        return true;
    }
}

void SeekNextBlock(CFile& fin){
    BlockIndex += 1;
    spdlog::info("SeekNextBlock BlockIndex={:d}", BlockIndex);
    g_HeadRevReader.updatePos();
    auto statB = g_InputEFS_FilePath.stat();
    long file_pos = BlockSize * BlockIndex;
    if(file_pos >= statB.st_size){
        throw CFileEofEx();
    }
    fin.seek(file_pos);
}


void ProcBlock(CFile& fin){
    CPath prefixedPath;
    uint32_t text_offset=0x0u;
    std::map<f3s_extptr_t, CPath> map_next, map_first;
    f3s_stat_t statB={0};
    uint32_t f3s_ext_type=0;
    f3s_extptr_t p_cur = {.logi_unit=1, .index=0xFFFFu};
    for(p_cur.index=0; p_cur.index < 0xFFFFu; ++p_cur.index){
        try{
            fin.seek(alignup(fin.tell(), g_Alignment));
            fmt::print("{}\n", p_cur.to_str());
            fmt::print("map_next={}\n", map_to_string(map_next));
            fmt::print("map_first={}\n", map_to_string(map_first));
            f3s_head_t head = g_HeadRevReader.read();

            f3s_ext_type = (head.status[0].to_value() & F3S_EXT_TYPE);
            fmt::print("f3s_ext_type={}\n", f3s_ext_type_to_str(f3s_ext_type));
            text_offset = static_cast<uint32_t>(fin.tell());
            fmt::print("text_offset={:08x}\n", text_offset);

            if ( f3s_ext_type == F3S_EXT_DIR ){
                if(head.text_size==0){
                    fmt::print("dirent (zero sized)\n");
                    map_first.erase(p_cur);           
                }
                else{
                    assert((sizeof(f3s_dirent_t)+sizeof(f3s_stat_t)) < head.text_size);
                    f3s_dirent_t dirent= fin.read<f3s_dirent_t>();  
                    CPath pathBuf = fin.read_str(F3S_NAME_ALIGN(dirent.namelen));
                    pathBuf.resize(dirent.namelen);
                    if( map_next.find(p_cur) != map_next.cend()){
                        CPath path_next_refer = map_next[p_cur];
                        map_next.erase(p_cur);
                        prefixedPath = path_next_refer.Parent()/pathBuf;
                    }else if( map_first.find(p_cur) != map_first.cend() ){
                        CPath path_first_refer = map_first[p_cur];
                        map_first.erase(p_cur);
                        prefixedPath = path_first_refer / pathBuf;
                    }
                    fmt::print("dirent.name=\"{}\", prefixedPath=\"{}\"\n", pathBuf, prefixedPath);
                    text_offset = static_cast<uint32_t>(fin.tell());
                    fmt::print("text_offset={:08x}\n", text_offset);
                    fin >> statB;
                    assert(statB.struct_size==sizeof(f3s_stat_t));
                    if(statB.mode.is_link()){
                        spdlog::info("is_link: prefixedPath=\"{}\"\n", (g_OutputDir / prefixedPath));
                        (g_OutputDir / prefixedPath).remove();
                    } else if(statB.mode.is_dir()){
                        spdlog::info("is_dir: prefixedPath=\"{}\"\n", (g_OutputDir / prefixedPath));
                        (g_OutputDir / prefixedPath).mkdir();
                    } else if(statB.mode.is_file()){
                        spdlog::info("is_file: prefixedPath=\"{}\"\n", (g_OutputDir / prefixedPath));
                        TouchFile(g_OutputDir / prefixedPath, statB);
                    }
                    if(dirent.first.is_valid()){
                        assert( map_first.find(dirent.first) == map_first.cend());
                        map_first[dirent.first] = prefixedPath;
                        fmt::print("first={}\n", dirent.first.to_str());
                    }
                }
            }else if( f3s_ext_type==F3S_EXT_FILE){
                if(map_first.find(p_cur) != map_first.cend()){
                    CPath path_first_refer = map_first[p_cur];
                    map_first.erase(p_cur);
                }
                if(statB.mode.is_link()){
                    CPath targetPath( fin.read_str(head.text_size) );
                    fmt::print("file symlink\n.text_size={:08x}\n.fileBuf=\"{}\"\n.prefixedPath=\"{}\"\n", head.text_size, targetPath, prefixedPath);
                    (g_OutputDir / prefixedPath).symlink(targetPath);
                }
                else if(statB.mode.is_file()){
                    std::vector<BYTE> data = fin.read_bytes(head.text_size);
                    fmt::print("file data\n.text_size={:08x}\n.prefixedPath=\"{}\"\n", data.size(), prefixedPath);
                    AppendFileData((g_OutputDir / prefixedPath), data.data(), head.text_size);
                }
            }else if(p_cur.index==F3S_UNIT_INFO_INDEX){
                f3s_unit_info_t unitInfo= fin.read<f3s_unit_info_t>();
                if(unitInfo.reserve != 0xFFFF || unitInfo.pad != 0xFF){
                    auto error_msg = fmt::format("unit_info invalid"); spdlog::error(error_msg); throw CFileException(error_msg);
                }
                fmt::print("unit_info\n.unit_pow2={:04x}\n.boot.logi_unit={:04x}\n.boot.index={:04x}\n", unitInfo.unit_pow2, unitInfo.boot.logi_unit, unitInfo.boot.index);
            }
            else if(p_cur.index==F3S_UNIT_LOGI_INDEX){
                f3s_unit_logi_t unitLogi={0};
                if(g_Alignment==4)
                    fin >> unitLogi;
                else if(g_Alignment==0x20){
                    auto unitLogi0x30 = fin.read<f3s_unit_logi_ecc<0x18>>();
                    assert(sizeof(unitLogi0x30) == 0x30);
                    unitLogi = unitLogi0x30.to_unit_logi();
                }
                else if (g_Alignment==0x40){
                    auto unitLogi0x50 = fin.read<f3s_unit_logi_ecc<0x38>>();
                    assert(sizeof(unitLogi0x50)==0x50);
                    unitLogi = unitLogi0x50.to_unit_logi();
                }
                fmt::print("unit_logi\n.age={:02x}\n.logi={:04x}\n", unitLogi.age,unitLogi.logi);
            } else if(p_cur.index==F3S_BOOT_INDEX){
                f3s_boot_t boot = fin.read<f3s_boot_t>();
                if(boot.rev_major==3 && boot.rev_minor==0 && 
#if __cplusplus==202002L
                std::string_view(boot.sig, 8)==F3S_SIG_STRING
#else
                0 == memcmp(boot.sig, F3S_SIG_STRING, sizeof(F3S_SIG_STRING))
#endif                
                ){
                    if(1<<boot.align_pow2 != g_Alignment){
                        auto err_msg=fmt::format("boot align_pow2 error"); spdlog::error(err_msg);throw CFileException(err_msg);
                    }
                    fmt::print("boot\n.unit_index={:04x}\n.unit_total={:04x}\n.unit_spare={:04x}\n.align_pow2={:04x}\n", boot.unit_index, boot.unit_total, boot.unit_spare, boot.align_pow2);                    
                }
                else{
                    auto err_msg = fmt::format("boot format error"); spdlog::error(err_msg); throw CFileException(err_msg);
                }
            }           
            if(head.next.is_valid()){
                assert(map_next.find(head.next) == map_next.cend());
                map_next[head.next] = prefixedPath;
                fmt::print("next={}\n", head.next.to_str()); 
            }
            
            if(head.status[0].last){
                p_cur.logi_unit += 1; 
                p_cur.index = 0xFFFFu;
                try{
                    SeekNextBlock(fin);
                }
                catch(CFileEofEx& ){
                    return;
                }
                continue;
            }
            printf("------------------------------------------------------------------------------\n");
        }
        catch(CFileEofEx&){
            return;
        }
        catch(SeekNextBlockEx&){
            try{
                SeekNextBlock(fin);
            }
            catch(CFileEofEx&){
                return;
            }
        }
    }
}

uint32_t get_Alignment(CFile& fin){
    uint16_t struct_size=0;
    fin.seek(0x20);
    fin >> struct_size;
    if(struct_size==0x30){
        return 0x20;
    }
    fin.seek(0x40);
    fin >> struct_size;
    if (struct_size==0x50){
        return 0x40;
    }
    assert("logical error");
    return 0;
}

int main(int argc, const char* argv[]){
    spdlog::set_level(spdlog::level::debug);

    if (argc<3){
        printf("usage: %s EFS_file_path Output_Dir\n", argv[0]);
        printf("       EFS_file_path: absolute path of EFS file\n"
               "       Output_Dir: path of Output_Dir\n");
        return 1;
    }
    g_InputEFS_FilePath = argv[1];
    g_OutputDir = argv[2];
    
    try{
        CFile fin(g_InputEFS_FilePath, "rb");
        f3s_unit_info_t unitInfo={0};
        fin >> unitInfo;
        if(unitInfo.unit_pow2 < 0x0A || 
            !(unitInfo.endian=='L' || unitInfo.endian=='B') ||
            unitInfo.pad !=0xFF || unitInfo.reserve!=0xFFFF){
            // block_size must be at least 1024 bytes (unit_pow2 >= 0x0A) 
            spdlog::error("f3s_unit_info_t invalid data");
            return 1;
        }
        BlockSize = (1<<unitInfo.unit_pow2);
        Endian = unitInfo.endian;
        
        f3s_head_t head = g_HeadRevReader.Init();
        if(head.mix.ecc[0] !=0xFF){
            g_Alignment = get_Alignment(fin);
        }
        long file_pos = BlockSize*BlockIndex;
        fin.seek(file_pos, SEEK_SET);
        ProcBlock(fin);
    }
    catch(CFileException& fex){
        spdlog::error("CFileException msg=\"{}\", errno={}", fex._msg, fex._errno);
        return fex._errno;
    }
    catch(std::exception& ex){
        spdlog::error("std::exception, what=\"{}\"", ex.what());
        return 1;
    }
    return 0;
}
