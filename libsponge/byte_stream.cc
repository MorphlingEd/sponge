#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity)
{
    _capacity = capacity;
    str.resize(capacity);
}

size_t ByteStream::write(const string &data)
{
    size_t bytesWritten = min(remaining_capacity(), data.size());
    if (writePtr + bytesWritten - 1 < _capacity)
        copy(data.begin(), data.begin() + bytesWritten, str.begin() + writePtr);
    else
    {
        copy(data.begin(), data.begin() + (_capacity - writePtr), str.begin() + writePtr);
        copy(data.begin() + (_capacity - writePtr), data.begin() + (bytesWritten - _capacity + writePtr), str.begin());
    }
    return bytesWritten;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const
{
    size_t bytesRead = min(len, write_count - read_count);
    size_t term = (readPtr + bytesRead) % _capacity;
    string res;

    if (term > readPtr)
        res = str.substr(readPtr, term);
    else
        res = str.substr(readPtr, _capacity - 1) + str.substr(0, term);

    return res;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len)
{
    readPtr = (readPtr + min(len, write_count - read_count)) % _capacity;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {

    size_t bytesRead = min(len, write_count - read_count);
    string res = peek_output(len);

    pop_output(len);
    read_count += bytesRead;
    if (read_count == write_count && _ended) _eof = true;

    return res;
}

void ByteStream::end_input() { _ended = true; }

bool ByteStream::input_ended() const { return _ended; }

size_t ByteStream::buffer_size() const { return write_count - read_count; }

bool ByteStream::buffer_empty() const { return read_count == write_count; }

bool ByteStream::eof() const { return _eof; }

size_t ByteStream::bytes_written() const
{
    return write_count;
}

size_t ByteStream::bytes_read() const
{
    return read_count;
}

size_t ByteStream::remaining_capacity() const
{
    return (_capacity - write_count + read_count);
}
