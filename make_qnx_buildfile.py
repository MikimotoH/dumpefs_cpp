import os
from pathlib import Path
import argparse

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('inputdir')
    parser.add_argument('-a', '--abs', action='store_true',
                        help='use absolute path for Right-Hand-Side of \'=\'')
    args = parser.parse_args()
    inputdir = Path(args.inputdir)
    use_abs:bool = args.abs

    block_size_added = False
    block_size = '128K'
    def blockSize():
        nonlocal block_size
        nonlocal block_size_added
        if not block_size_added:
            block_size_added=True
            return f'block_size={block_size}'
        else:
            return ''

    for root, dirs, files in os.walk(inputdir):
        for name in files:
            fpath = Path(root)/Path(name)
            
            if fpath.is_symlink():
                stat = os.stat(fpath, follow_symlinks=False)
                print(f'[uid=0 gid=0 type=link perms={stat.st_mode&0o7777:04o} {blockSize()}]')
                print('/'+ str(fpath.relative_to(inputdir)) + '=',end='')
                targetPath = os.readlink(fpath)
                print(f'{targetPath}')
            elif fpath.is_file():
                stat = fpath.stat()
                print(f'[uid=0 gid=0 type=file perms={stat.st_mode&0o7777:04o} {blockSize()}]')
                print('/'+ str(fpath.relative_to(inputdir)) + '=',end='')
                if use_abs:
                    print(fpath.resolve())
                else:
                    print(fpath.relative_to(inputdir))
        for name in dirs:
            fpath = Path(root)/Path(name)
            stat = fpath.stat()
            print(f'[uid=0 gid=0 type=dir dperms={stat.st_mode&0o7777:04o} {blockSize()}]')
            print('/'+ str(fpath.relative_to(inputdir)))

if __name__=='__main__':
    main()
