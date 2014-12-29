#include <assert.h>
#include <fstream>
#include <limits.h>
#include "./path.h"

namespace xtd {

//------------------------------------------------------------------------------

static void appendTrailingSlash(std::string& path)
{
    if (path.back() != '/') path += '/';
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

static void removeTrailingSlash(std::string& path)
{
    if (path.back() == '/') path.resize(path.size() - 1);
}

//------------------------------------------------------------------------------

path::path()
{
    reset();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

path::path(const char* path, const size_t len)
{
    assert(path);
    assert(len <= strlen(path));
    reset(std::string(path, len));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

path::path(const char* path)
{
    assert(path);
    reset(path ? path : "");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

path::path(std::string path)
{
    reset(std::move(path));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

path::path(const path& p)
: _path(p._path)
, _stats(p._stats)
, _exists(p._exists)
{}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

path& path::operator = (const path& p)
{
    reset();
    
    _path   = p._path;
    _stats  = p._stats;
    _exists = p._exists;

    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

path::~path()
{
    reset();
}

//------------------------------------------------------------------------------

const bool path::operator == (const path& p) const
{
    return _path == p._path;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const bool path::operator != (const path& p) const
{
    return _path != p._path;
}

//------------------------------------------------------------------------------

const bool path::operator <= (const path& p) const
{
    return _path <= p._path;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const bool path::operator >= (const path& p) const
{
    return _path >= p._path;
}

//------------------------------------------------------------------------------

const bool path::operator < (const path& p) const
{
    return _path < p._path;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const bool path::operator > (const path& p) const
{
    return _path > p._path;
}

//------------------------------------------------------------------------------

std::string path::extension() const
{
    const size_t ext_index = _path.rfind(".");

    return (~ext_index) ? _path.substr(ext_index + 1) : std::string();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

std::string path::name() const
{
    const size_t path_size = _path.size();

    if (path_size == 0) return "";

    if (path_size == 1) return _path;

    const size_t dir_index = _path.rfind("/", path_size - 2);

    if (dir_index == std::string::npos)
        return _path;

    const size_t sub_size = path_size - dir_index;

    if (_path.back() == '/')
        return _path.substr(dir_index + 1, sub_size - 2);

    return _path.substr(dir_index + 1);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

std::string path::stem() const
{
    size_t dir_index = _path.rfind("/");

    if (dir_index == std::string::npos)
        dir_index = 0;

    size_t ext_index = _path.rfind(".");

    if (ext_index == std::string::npos)
        ext_index = _path.size();

    return _path.substr(dir_index, ext_index);
}

//------------------------------------------------------------------------------

path path::parent() const
{
    const size_t lastIndex = _path.size() - 1;

    const size_t lastSlash = _path.find_last_of('/', lastIndex - 1);

    const bool foundLastSlash = (lastSlash != std::string::npos);

    return (foundLastSlash) ? path(c_str(), lastSlash + 1) : path();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

path path::root() const
{
    const size_t firstSlash = _path.find_first_of('/');

    const bool foundFirstSlash = (firstSlash != std::string::npos);

    return (foundFirstSlash) ? path(c_str(), firstSlash + 1) : path();
}

//------------------------------------------------------------------------------

const bool path::create_directory()
{
    if (mkdir(c_str(), S_IRWXG | S_IRWXO | S_IRWXU))
        return false;

    reset(std::move(_path));

    return exists();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const bool path::exists() const
{
    return _exists;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const bool path::is_directory() const
{
    return exists() and S_ISDIR(_stats.st_mode);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const bool path::is_file() const
{
    return exists() and S_ISREG(_stats.st_mode);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const bool path::is_pipe() const
{
    return exists() and S_ISFIFO(_stats.st_mode);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const bool path::is_symlink() const
{
    return exists() and S_ISLNK(_stats.st_mode);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

time_t path::modified() const
{
    return exists() ? _stats.st_mtime : time_t();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const path path::real_path() const
{
    char buffer[PATH_MAX];

    const char* realpath = ::realpath(c_str(), buffer);

    return realpath ? path(realpath) : path();
}

//------------------------------------------------------------------------------

path::iterator path::begin() const
{
    return path::iterator(is_directory() ? c_str() : nullptr);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

path::iterator path::end() const
{
    return path::iterator();
}

//------------------------------------------------------------------------------

std::string path::read()
{
    if (not is_file())
        return {};

    std::ifstream stream(c_str());

    stream.seekg(0, std::ios_base::end);

    const auto size = size_t(stream.tellg());

    stream.seekg(0, std::ios_base::beg);

    std::string string;

    string.resize(size);

    stream.read(&string[0], size);

    return string;
}

//------------------------------------------------------------------------------

void path::write(std::string s)
{
    assert(not is_directory());

    std::ofstream stream(c_str());

    stream << s;
}

//------------------------------------------------------------------------------

void path::reset()
{
    reset("");
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void path::reset(std::string path)
{
    _path = std::move(path);

    const bool isNotRootPath = (_path != "/");

    if (isNotRootPath)
        removeTrailingSlash(_path);

    memset(&_stats, 0, sizeof(_stats));

    _exists = _path.size() ? (stat(c_str(), &_stats) == 0) : false;

    if (isNotRootPath and is_directory())
        appendTrailingSlash(_path);
}

//==============================================================================

path::iterator::iterator(const std::string& root)
: _root { root }
{
    if (_root.size())
    {
        _dir = opendir(_root.c_str());

        if (_dir) operator++();
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

path::iterator::~iterator()
{
    reset();
}

//------------------------------------------------------------------------------

const bool path::iterator::operator == (const iterator& it) const
{
    return _path == it._path;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

const bool path::iterator::operator != (const iterator& it) const
{
    return not this->operator==(it);
}

//------------------------------------------------------------------------------

path::iterator& path::iterator::operator++()
{
    assert(_dir);

skip_entry:

    if (dirent* entry = readdir(_dir))
    {
        const char c0 = entry->d_name[0];
        const char c1 = entry->d_name[1];

        // skip "." and ".." paths
        if (c0 == '.' and (c1 == 0 or c1 == '.'))
            goto skip_entry;

        _path = path(_root + entry->d_name);
    }
    else
    {
        reset();
    }

    return *this;
}

//------------------------------------------------------------------------------

void path::iterator::reset()
{
    if (_dir)
    {
        closedir(_dir);
        _dir = nullptr;
        _path.reset();
    }
}

//==============================================================================

} // namespace path
