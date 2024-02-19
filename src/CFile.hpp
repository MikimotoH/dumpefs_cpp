#pragma once

#include <stdio.h>
#include <string>
#include <exception>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/bundled/format.h>

struct CFileException : public std::exception{
public:
    std::string _msg;
    int _errno;
    CFileException(const char* msg, int error_code=22)
        :_msg(msg), _errno(error_code){
    }
    CFileException(const std::string& msg, int error_code=22)
        :_msg(msg), _errno(error_code){
    }    
    virtual const char*
    what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW{
        return this->_msg.c_str();
    }
};

static inline std::string string_replace_all(const std::string& str, const std::string& search, const std::string& replace){
    size_t pos = str.find(search, pos);
    if(pos == std::string::npos){
        return str;
    }
    std::string strBuf = str;
    while ((pos = str.find(search, pos)) != std::string::npos) {
        strBuf.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return strBuf;
}

struct CPath : public std::string{
    static inline std::string remove_trailing_slash(const std::string& str){
#if __cplusplus == 202002L
        if(str.size() > 1 && str.ends_with('/')){
            return str.substr(0, str.size()-1);
        }else{
            return str;
        }
#else
        if(str.size() > 1 && str[str.size()-1]=='/'){
            return str.substr(0, str.size()-1);
        }else{
            return str;
        }
#endif        
    }
    static inline std::string str_to_validate_path(const std::string& s){
        return string_replace_all(remove_trailing_slash(s), "//", "/");
    }

    CPath(){
    }

    CPath(const std::string& s){
        this->std::string::operator=(str_to_validate_path(s));
    }
    CPath(const char* s){
        this->std::string::operator=(str_to_validate_path(std::string(s)));
    }

    // copy constructor
    CPath(const CPath& other)
    :std::string::basic_string(other){}

    // Move Constructor
    CPath(CPath&& other)
    :std::string::basic_string(std::move(other)){}

    CPath(std::string&& other)
    :std::string::basic_string(std::move(other)){
        this->std::string::operator=(str_to_validate_path(*this));
    }
    
    CPath& operator=(const CPath& other){
        this->std::string::operator=((const std::string&)other);
        return *this;
    }
    CPath& operator=(const std::string& other){
        this->std::string::operator=(str_to_validate_path(other));
        return *this;
    }
    CPath& operator=(const char* s){
        this->std::string::operator=(str_to_validate_path(std::string(s)));
        return *this;
    }

    CPath operator /(const CPath& rhs)const{
        if(this->length()>0 && rhs.length()>0){
            auto ret = fmt::format("{}/{}", this->c_str(), rhs.c_str());
            return CPath(str_to_validate_path(ret));
        }else if(this->length()==0 && rhs.length()>0){
            return rhs;
        }else if(this->length()>0 && rhs.length()==0){
            return *this;
        }
        else{
            return *this;
        }
    }

    CPath Parent()const{
        std::size_t slash_pos = this->find_last_of('/');
        if(slash_pos != this->npos){
            return this->substr(0, slash_pos);
        }else{
            return CPath();
        }
    }

    struct stat64 stat()const{
        struct stat64 statB={0};
        int res = ::stat64(this->c_str(), &statB);
        if(res!=0){
            res = errno;
            auto err_msg=fmt::format("stat64({}) errno={} ", *this, res);
            throw CFileException(err_msg, res);
        }
        return statB;
    }

    bool is_dir()const{
        struct stat64 statB = this->stat();
        return statB.st_mode & __S_IFDIR;
    }
    bool is_file()const{
        struct stat64 statB = this->stat();
        return statB.st_mode & __S_IFREG;
    }
    bool is_link()const{
        struct stat64 statB = this->stat();
        return statB.st_mode & __S_IFLNK;
    }

    void mkdir(bool exist_ok=true)const {
        std::string tmpStr = *this;
        char *p = nullptr;
        long res = 0;
        char* tmp= const_cast<char*>(tmpStr.data());

        size_t len = tmpStr.length();
        if (tmp[len - 1] == '/')
            tmp[len - 1] = 0;
        for (p = tmp + 1; *p; p++)
            if (*p == '/') {
                *p = 0;
                res = ::mkdir(tmp, 0755);
                *p = '/';
            }
        res = ::mkdir(tmp, 0755);
        if(res!=0){
            res = errno;
            if( res != EEXIST || !exist_ok){
                auto err_msg = fmt::format("::mkdir(\"{}\") errno={}", tmp, res);
                spdlog::error(err_msg);
                throw CFileException(err_msg, res);
            }
        }
    }



    void symlink(const CPath& target){
        int res = ::symlink(target.c_str(), this->c_str());
        if(res!=0){
            res = errno;
            auto err_msg = fmt::format("symlink(\"{}\", \"{}\") errno={}", target, *this, res);
            spdlog::error(err_msg);
            throw CFileException(err_msg, res);
        }
    }

    void remove(void) noexcept(true) {
        int res = ::remove(this->c_str());
        if(res!=0){
            res = errno;
            spdlog::warn("remove {} errno={:d}", dynamic_cast<std::string&>(*this), res);
        }
    }
};


struct CFileEofEx : public std::exception{
public:
    CFileEofEx(void){
    }
};

struct CFile{
public:
    FILE* _fp;
    CPath _fname;
    
    void open(const CPath& filename, const char* mode){
        this->_fp = fopen(filename.c_str(), mode);
        if(!this->_fp){
            int res = errno;
            auto error_msg = fmt::format("fopen(\"{}\", \"{}\") errno={}", filename, mode, res);
            spdlog::error(error_msg);
            throw CFileException(error_msg, res);
        }
        this->_fname = filename;
    }
    
    CFile(const CPath& filename, const char* mode)
    :_fname(filename), _fp(nullptr){
        this->open(filename, mode);
    }

    CFile(void)
    :_fp(nullptr){
    }

    void close(){
        if(this->_fp){
            fclose(this->_fp);
            this->_fp = nullptr;
            this->_fname.clear();
        }
    }
    ~CFile(){
        this->close();
    }

    void seek(long offset, int whence=SEEK_SET){
        int res = fseek(this->_fp, offset, whence);
        if(res!=0){
            res = errno;
            auto error_msg = fmt::format("fseek(\"{}\", {:08x}, {}) errno={}", this->_fname, offset, whence, res);
            spdlog::error(error_msg);
            throw CFileException(error_msg, res);
        }
    }
    bool operator !()const{
        return !this->_fp;
    }
    operator bool()const{
        return this->_fp != nullptr;
    }

    long tell(void)const{
        long res = ftell(this->_fp);
        if(res == -1){
            res = errno;
            auto error_msg = fmt::format("ftell(\"{}\") errno={}", this->_fname, res);
            spdlog::error(error_msg);
            throw CFileException(error_msg, res);
        }
        return res;
    }

    void read(void* pBuf,size_t byte_len){
        if(byte_len==0)
            return;
        long offset = this->tell();
        size_t res =fread(pBuf, byte_len, 1, this->_fp);
        if(res!=1){
            if(feof(this->_fp))
                throw CFileEofEx();
            auto error_msg = fmt::format("fread(\"{}\", {:08x}) offset={:08x} errno={}", this->_fname, byte_len, offset, res);
            spdlog::error(error_msg);
            throw CFileException(error_msg, res);
        }
    }
    std::string read_str(size_t byte_len){
        std::string container(byte_len, static_cast<char>('\x00'));
        if(byte_len!=0)
            this->read(const_cast<char*>(container.data()), byte_len);
        return container;
    }
    std::vector<BYTE> read_bytes(size_t byte_len){
        std::vector<BYTE> container(byte_len, static_cast<BYTE>(0));
        if(byte_len!=0)
            this->read(const_cast<BYTE*>(container.data()), byte_len);
        return container;
    }

    template<typename T, typename ContainerType = std::basic_string<T>>
    ContainerType read(size_t byte_len){
        ContainerType container(byte_len/sizeof(T), static_cast<T>('\x00'));
        this->read(container.data(), byte_len);
        return container;
    }

    template<typename T>
    T read(){
        T t;
        this->read(&t, sizeof(t));
        return t;
    }

    void write(const void* pBuf,size_t byte_len){
        long offset = ftell(this->_fp);
        size_t res = fwrite(pBuf, byte_len, 1, this->_fp);
        if(res!=1){
            res = errno;
            auto error_msg = fmt::format("fwrite(\"{}\", {:08x}) offset={:08x} errno={}", this->_fname, byte_len, offset, res);
            spdlog::error(error_msg);
            throw CFileException(error_msg, res);
        }
    }
};


template<typename T>
inline CFile& operator>>(CFile& fin, T& data){
    fin.read(&data, sizeof(data));
    return fin;
}

