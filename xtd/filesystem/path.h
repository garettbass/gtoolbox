#pragma once
#include <string>
#include <dirent.h>
#include <sys/stat.h>

namespace xtd {

    //--------------------------------------------------------------------------
    // class path provides an easy way to query paths and iterate directories.
    //
    // sample usage:
    //
    //     if (path dataPath = "/data")           // evaluate dataPath.exists()
    //         for (auto& subpath : dataPath)     // iterate over directory
    //             if (subpath.is_file())         // print names of files
    //                 printf("file name: %s\n", subpath.stem());

    class path final
    {
        class iterator;

        typedef struct stat stats;
        
        std::string  _path;
        stats        _stats;
        bool         _exists;

    public: // structors

        path();

        path(const char* path, size_t len);

        path(const char* path);

        path(std::string path);

        path(const path&);

        path& operator = (const path&);

       ~path();

    public: // operators

        explicit operator const bool() const { return exists(); }

        operator const std::string&() const { return _path; }

        const bool operator == (const path& p) const;
        const bool operator != (const path& p) const;

        const bool operator <= (const path& p) const;
        const bool operator >= (const path& p) const;

        const bool operator < (const path& p) const;
        const bool operator > (const path& p) const;

    public: // properties

        const char* c_str() const { return _path.c_str(); }

        const bool exists() const;

        std::string extension() const; // path("bar.txt").extension() == "txt"

        std::string name() const;      // path("bar.txt").name() == "bar.txt"

        std::string stem() const;      // path("bar.txt").stem() == "bar"

        const bool is_directory() const;

        const bool is_file() const;

        const bool is_pipe() const;

        const bool is_symlink() const;

        time_t modified() const;

        path parent() const;

        const path real_path() const;

        path root() const;

        const std::string& str() const { return _path; }

    public: // iterators

        iterator begin() const;
        
        iterator end() const;

    public: // methods

        const bool create_directory();

        std::string read();

        void reset();

        void reset(std::string);

        void write(std::string);

    };

    inline
    path operator / (const path& a, const path& b)
    {
        return { a.str() + b.str() };
    }

    //--------------------------------------------------------------------------

    class path::iterator final
    {
        const std::string _root;
        DIR*              _dir = nullptr;
        path              _path;

    public: // structors

        iterator() = default;

        iterator(const std::string&);

       ~iterator();

    public: // operators

        const bool operator == (const iterator& it) const;
        const bool operator != (const iterator& it) const;

        const path& operator *  () const { return  _path; }
        const path* operator -> () const { return &_path; }

        iterator& operator ++ ();

    private: // implementation

        void reset();

    };

    //--------------------------------------------------------------------------

} // namespace xtd
