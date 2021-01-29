#pragma once
#include <cstdint>
#include <limits>
#include <string>
#include <array>
#include <rttr/registration_friend>
namespace native
{

    namespace details
    {
        template <size_t Bytes>
        using impl_size_type_helper = std::conditional_t<Bytes == 1, std::uint8_t, std::conditional_t<Bytes == 2, std::uint16_t, std::uint32_t>>;

        template <typename... Args>
        using enable_if_conjunction = std::enable_if_t<std::conjunction_v<Args...>, bool>;

        template <typename T, typename = void>
        struct is_iterator : std::false_type
        {
        };

        template <typename T>
        struct is_iterator<T, typename std::enable_if_t<!std::is_same_v<typename std::iterator_traits<T>::value_type, void>>> : std::true_type
        {
        };

    } // namespace details

    template <typename CharT,
        size_t MaxSize,
        typename Traits = std::char_traits<CharT>>
        class basic_string
    {
        //**********************************************************************************
        // Member Types
        //**********************************************************************************

    public:
        using impl_size_type = details::impl_size_type_helper<sizeof(CharT)>;

        using traits_type = Traits;
        using value_type = CharT;
        using size_type = size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        //**********************************************************************************
        // Constants
        //**********************************************************************************

        static constexpr size_type npos = static_cast<size_type>(-1);

        //**********************************************************************************
        // Constructors
        //**********************************************************************************

        // Default (1)
        constexpr basic_string() noexcept
        {
            clear();
        }

        // Construct with count copies of ch (2)
        constexpr basic_string(size_type count, CharT ch)
        {
            assign(count, ch);
        }

        // Construct string with substr (3)
        constexpr basic_string(const basic_string& others,
            size_type pos)
            : basic_string(std::basic_string_view<CharT, Traits>{others}.substr(pos)) {}

        template <size_t OtherMaxSize>
        constexpr basic_string(const basic_string<CharT, OtherMaxSize, Traits>& others,
            size_type pos)
            : basic_string(std::basic_string_view<CharT, Traits>{others}.substr(pos)) {}

        // Construct string with substr w Count (3)
        constexpr basic_string(const basic_string& others,
            size_type pos,
            size_type count)
            : basic_string(std::basic_string_view<CharT, Traits>{others}.substr(pos, count)) {}

        template <size_t OtherMaxSize>
        constexpr basic_string(const basic_string<CharT, OtherMaxSize, Traits>& others,
            size_type pos,
            size_type count)
            : basic_string(std::basic_string_view<CharT, Traits>{others}.substr(pos, count)) {}

        // (4)
        constexpr basic_string(const_pointer p,
            size_type count)
        {
            assign(p, count);
        }

        // (5)
        constexpr basic_string(const_pointer p) : basic_string(p, traits_type::length(p))
        {
        }

        // Iterator Construction (6)
        template <typename InputIt, details::enable_if_conjunction<details::is_iterator<InputIt>> = true>
        constexpr basic_string(InputIt first,
            InputIt last)
        {
            assign(first, last);
        }

        // Copy (7)
        constexpr basic_string(const basic_string& others) = default;

        // Move (8)
        constexpr basic_string(basic_string&& others) = default;

        // Initializer List Construction (9)
        constexpr basic_string(std::initializer_list<CharT> ilist)
        {
            assign(ilist.begin(), ilist.size());
        }

        // Conversion Construction (10)
        template <typename T,
            details::enable_if_conjunction<std::is_convertible<const T&, std::basic_string_view<CharT, Traits>>> = true>
            constexpr basic_string(const T& others)
            : basic_string(std::basic_string_view<CharT, Traits>(others)) {}

        // Conversion Construction (11)
        template <typename T,
            details::enable_if_conjunction<std::is_convertible<const T&, std::basic_string_view<CharT, Traits>>> = true>
            constexpr basic_string(const T& others,
                size_type pos,
                size_type count)
            : basic_string(std::basic_string_view<CharT, Traits>(others).substr(pos, count)) {}

        // String View Construction
        constexpr explicit basic_string(std::basic_string_view<CharT, Traits> view)
            : basic_string(view.data(), view.size()) {}

        //**********************************************************************************
        // Assignment Operator
        //**********************************************************************************

        // Copy Assignment (1)
        constexpr basic_string& operator=(const basic_string& others)
        {
            return assign(others.data(), others.size());
        }

        template <size_t OtherMaxSize>
        constexpr basic_string& operator=(const basic_string<CharT, OtherMaxSize, Traits>& others)
        {
            return assign(others.data(), others.size());
        }

        // Move Assignment (2)
        constexpr basic_string& operator=(basic_string&& others)
        {
            traits_type::move(data(), others.data(), others.size());
            return *this;
        }

        // Copy Assignment (3)
        constexpr basic_string& operator=(const_pointer p)
        {
            return assign(p);
        }

        // Replace Assignment with ch (4)
        constexpr basic_string& operator=(value_type ch)
        {
            return assign(1, ch);
        }

        // Initializer List Assigniment (5)
        constexpr basic_string& operator=(std::initializer_list<CharT> ilist)
        {
            return assign(ilist.begin(), ilist.size());
        }

        // Conversion Assignment (6)
        template <typename T, details::enable_if_conjunction<std::is_convertible<const T&, std::basic_string_view<CharT, Traits>>> = true>
        constexpr basic_string& assign(T& others)
        {
            this = std::basic_string_view<CharT, Traits>(others);
            return *this;
        }

        // String View Assignment
        constexpr basic_string& operator=(std::basic_string_view<CharT, Traits> view)
        {
            return assign(view);
        }

        //**********************************************************************************
        // Assignment Member Function
        //**********************************************************************************

        // Assign count copies of ch (1)
        constexpr basic_string& assign(size_type count,
            CharT ch)
        {
            set_size(count);
            traits_type::copy(count, data(), ch);
            return *this;
        }

        // Assign copy of str (2)
        constexpr basic_string& assign(const basic_string& str)
        {
            return assign(str.data(), str.size());
        }

        template <size_t OtherMaxSize>
        constexpr basic_string& assign(const basic_string<CharT, OtherMaxSize, Traits>& str)
        {
            return assign(str.data(), str.size());
        }

        // Assign copy of str from pos(3)
        constexpr basic_string& assign(const basic_string& str,
            size_type pos,
            size_type count = npos)
        {
            return assign(std::basic_string_view<CharT, Traits>(str).substr(pos, count));
        }

        template <size_t OtherMaxSize>
        constexpr basic_string& assign(const basic_string<CharT, OtherMaxSize, Traits>& str,
            size_type pos,
            size_type count = npos)
        {
            return assign(std::basic_string_view<CharT, Traits>(str).substr(pos, count));
        }

        // Assign and move str into *this (4)
        constexpr basic_string& assign(const basic_string&& str)
        {
            return *(this = str);
        }

        // Assign copy of str in range of p, p+count (5)
        constexpr basic_string& assign(const_pointer p, size_type count) noexcept
        {
            set_size(count);
            traits_type::copy(data(), p, count);
            return *this;
        }

        // Assign copy of str in range of p (6)
        constexpr basic_string& assign(const_pointer p) noexcept
        {
            return assign(p, traits_type::length(p));
        }

        // Assign copy of str in range of first, last (7)
        template <typename InputIt, details::enable_if_conjunction<details::is_iterator<InputIt>> = true>
        constexpr basic_string& assign(InputIt first, InputIt last)
        {
            set_size(std::distance(first, last));
            std::copy(first, last, data());
            return *this;
        }

        // Assign from initializer list (8)
        constexpr basic_string& assign(std::initializer_list<CharT> ilist)
        {
            return assign(ilist.begin(), ilist.size());
        }

        // (9)
        template <typename T,
            details::enable_if_conjunction<
            std::is_convertible<const T&, std::basic_string_view<CharT, Traits>>, std::negation<std::is_convertible<const T&, const CharT*>>> = true>
            constexpr basic_string& assign(const T& str)
        {
            return assign(std::basic_string_view<CharT, Traits>(str));
        }

        // (10)
        template <typename T,
            details::enable_if_conjunction<
            std::is_convertible<const T&, std::basic_string_view<CharT, Traits>>, std::negation<std::is_convertible<const T&, const CharT*>>> = true>
            constexpr basic_string& assign(const T& str, size_type pos, size_type count = npos)
        {
            return assign(std::basic_string_view<CharT, Traits>(str).substr(pos, count));
        }

        // Assign from string view
        constexpr basic_string& assign(std::basic_string_view<CharT, Traits> view)
        {
            return assign(view.data(), view.size());
        }

        //**********************************************************************************
        // Element Access
        //**********************************************************************************

        constexpr reference at(size_type pos)
        {
            if (pos >= size())
                throw std::out_of_range("basic_string::at");
            return (*this)[pos];
        }

        constexpr const_reference at(size_type pos) const
        {
            if (pos >= size())
                throw std::out_of_range("basic_string::at");
            return (*this)[pos];
        }

        constexpr reference operator[](size_type pos)
        {
            return *(begin() + pos);
        }

        constexpr const_reference operator[](size_type pos) const
        {
            return *(begin() + pos);
        }

        constexpr reference front()
        {
            return (*this)[0];
        }

        constexpr const_reference front() const
        {
            return (*this)[0];
        }

        constexpr reference back()
        {
            return (*this)[size() - 1];
        }

        constexpr const_reference back() const
        {
            return (*this)[size() - 1];
        }

        constexpr pointer data() noexcept
        {
            return chars.data();
        }

        constexpr const_pointer data() const noexcept
        {
            return chars.data();
        }

        constexpr const_pointer c_str() const noexcept
        {
            return data();
        }

        constexpr operator std::basic_string_view<CharT, Traits>() const noexcept
        {
            return { data(), size() };
        }

        //**********************************************************************************
        // Iterators
        //**********************************************************************************

        constexpr iterator begin()
        {
            return data();
        }
        constexpr const_iterator begin() const
        {
            return data();
        }
        constexpr iterator end()
        {
            return data() + size();
        }
        constexpr const_iterator end() const
        {
            return data() + size();
        }

        constexpr reverse_iterator rbegin()
        {
            return reverse_iterator(end());
        }
        constexpr const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(end());
        }
        constexpr reverse_iterator rend()
        {
            return reverse_iterator(begin());
        }
        constexpr const_reverse_iterator rend() const
        {
            return const_reverse_iterator(begin());
        }

        constexpr const_iterator cbegin() const
        {
            return data();
        }
        constexpr const_iterator cend() const
        {
            return data() + size();
        }
        constexpr const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator(cend());
        }
        constexpr const_reverse_iterator crend() const
        {
            return const_reverse_iterator(cbegin());
        }

        //**********************************************************************************
        // Capacity
        //**********************************************************************************

        constexpr bool empty() const
        {
            return size() == 0;
        }
        constexpr size_type size() const
        {
            return static_cast<impl_size_type>(chars.back());
        }
        constexpr size_type length() const
        {
            return size();
        }
        constexpr size_type max_size() const
        {
            return MaxSize - 2;
        }
        constexpr size_type capacity() const
        {
            return max_size()
        }

        //**********************************************************************************
        // Operations
        //**********************************************************************************

        constexpr void clear() noexcept
        {
            set_size(0);
        }

        // insert
        // erase

        constexpr void push_back(CharT ch)
        {
            append(static_cast<size_type>(1), ch);
        }

        // pop_back

        // Append copies of ch (1)
        constexpr basic_string& append(size_type count, value_type ch)
        {
            resize(size() + count, ch);
            return *this;
        }

        // Append str (2)
        constexpr basic_string& append(const basic_string& str)
        {
            return append(str.data(), str.size());
        }

        // Append substring (3)
        constexpr basic_string& append(const basic_string& str, size_type pos, size_type count)
        {
            return append(std::basic_string_view<CharT, Traits>(str).substr(pos, count));
        }

        // Append character in range w count(4)
        constexpr basic_string& append(const_pointer p, size_type count)
        {
            const auto sz = size();
            set_size(sz + count);
            traits_type::copy(data() + sz, p, count);
            return *this;
        }

        // Append character in range (5)
        constexpr basic_string& append(const_pointer p)
        {
            return append(p, traits_type::length(p));
        }

        // Append character in range iterator (6)
        template <typename InputIt, details::enable_if_conjunction<details::is_iterator<InputIt>> = true>
        constexpr basic_string& append(InputIt first, InputIt last)
        {
            const auto sz = size();
            set_size(sz + std::distance(first, last));
            std::copy(first, last, data() + sz);
            return *this;
        }

        // Append character using initializer list (7)
        constexpr basic_string& append(std::initializer_list<CharT> ilist)
        {
            return append(ilist.begin(), ilist.end());
        }

        // Append character with conversion (8)
        template <typename T,
            details::enable_if_conjunction<
            std::is_convertible<const T&, std::basic_string_view<CharT, Traits>>, std::negation<std::is_convertible<const T&, const CharT*>>> = true>
            constexpr basic_string& append(const T& str)
        {
            return append(std::basic_string_view<CharT, Traits>(str));
        }

        // Append character with conversion with substr(9)
        template <typename T,
            details::enable_if_conjunction<
            std::is_convertible<const T&, std::basic_string_view<CharT, Traits>>, std::negation<std::is_convertible<const T&, const CharT*>>> = true>
            constexpr basic_string& append(const T& str, size_type pos, size_type count = npos)
        {
            return append(std::basic_string_view<CharT, Traits>(str).substr(pos, count));
        }

        constexpr basic_string& append(std::basic_string_view<CharT, Traits> view)
        {
            return append(view.data(), view.size());
        }

        // Operator+= (1)
        constexpr basic_string& operator+=(const basic_string& str)
        {
            return append(str);
        }

        template <size_t OtherMaxSize>
        constexpr basic_string& operator+=(const basic_string<CharT, OtherMaxSize, Traits>& str)
        {
            return append(str);
        }

        // Operator+= (2)
        constexpr basic_string& operator+=(value_type ch)
        {
            push_back(ch);
            return *this;
        }

        // Operator+= (3)
        constexpr basic_string& operator+=(const_pointer p)
        {
            return append(p);
        }

        // Operator+= (4)
        constexpr basic_string& operator+=(std::initializer_list<CharT> ilist)
        {
            return append(ilist);
        }

        // Operator+= (5)
        template <typename T>
        constexpr basic_string& operator+=(const T& others)
        {
            return append(others);
        }

        constexpr basic_string& operator+=(std::basic_string_view<CharT, Traits> view)
        {
            return append(view);
        }

        // compare
        // starts_with
        // ends_with
        // contains
        // replace

        constexpr basic_string substr(size_type pos = 0) const
        {
            return basic_string(data() + pos);
        }

        constexpr basic_string substr(size_type pos, size_type count) const
        {
            return basic_string(data() + pos, count);
        }

        // copy

        // resize (1)
        constexpr void resize(size_type count)
        {
            resize(count, value_type{});
        }

        // resize (2)
        constexpr void resize(size_type count, value_type ch)
        {
            const auto sz = size();
            set_size(sz);
            if (count > sz)
                traits_type::assign(data() + sz, count - size, ch);
        }

        constexpr void swap(basic_string& others)
        {
            std::swap(chars, others.chars);
        }

        std::array<value_type,MaxSize> get_array() const
        {
            return chars;
        }

    	void set_array(std::array<value_type, MaxSize> arr)
        {
            chars = arr;
        }
    	
        //**********************************************************************************
        // Data Member
        //**********************************************************************************
    private:
        constexpr void set_size(size_type size)
        {
            if (size > max_size())
                throw std::length_error("native::basic_string: size() > max_size()");
            chars[size] = '\0';
            chars.back() = static_cast<impl_size_type>(size);
        }

        //**********************************************************************************
        // Data Member
        //**********************************************************************************
    public:
        std::array<value_type, MaxSize> chars;

        //RTTR_REGISTRATION_FRIEND
    };

    //**********************************************************************************
    // Non-member function
    //**********************************************************************************

    // operator+ TBC
    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr std::basic_string<CharT, Traits> operator+(const basic_string<CharT, MaxSize, Traits>& lhs, const CharT* rhs)
    {
        return std::basic_string(lhs.data(), lhs.size()) + rhs;
    }

    template <typename CharT, size_t MaxSize, typename Traits, typename T, details::enable_if_conjunction<std::is_convertible<const T&, std::basic_string_view<CharT, Traits>>> = true>
    constexpr std::basic_string<CharT, Traits> operator+(const basic_string<CharT, MaxSize, Traits>& lhs, const T& rhs)
    {
        return lhs + std::basic_string_view<CharT, Traits>(rhs).data();
    }

    // Equality ==
    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator==(const basic_string<CharT, MaxSize, Traits>& lhs, const basic_string<CharT, MaxSize, Traits>& rhs)
    {
        return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs));
    }

    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator==(const CharT* lhs, const basic_string<CharT, MaxSize, Traits>& rhs)
    {
        return std::equal(lhs, lhs + Traits::length(lhs), std::begin(rhs), std::end(rhs));
    }

    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator==(const basic_string<CharT, MaxSize, Traits>& lhs, const CharT* rhs)
    {
        return std::equal(std::begin(lhs), std::end(lhs), rhs, rhs + Traits::length(rhs));
    }

    // Equality !=
    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator!=(const basic_string<CharT, MaxSize, Traits>& lhs, const basic_string<CharT, MaxSize, Traits>& rhs)
    {
        return !(lhs == rhs);
    }

    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator!=(const CharT* lhs, const basic_string<CharT, MaxSize, Traits>& rhs)
    {
        !(lhs == rhs);
    }

    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator!=(const basic_string<CharT, MaxSize, Traits>& lhs, const CharT* rhs)
    {
        !(lhs == rhs);
    }

    // Operator <
    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator<(const basic_string<CharT, MaxSize, Traits>& lhs, const basic_string<CharT, MaxSize, Traits>& rhs)
    {
        return std::lexicographical_compare(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs));
    }

    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator<(const CharT* lhs, const basic_string<CharT, MaxSize, Traits>& rhs)
    {
        return std::lexicographical_compare(lhs, lhs + Traits::length(lhs), std::begin(rhs), std::end(rhs));
    }

    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator<(const basic_string<CharT, MaxSize, Traits>& lhs, const CharT* rhs)
    {
        return std::lexicographical_compare(std::begin(lhs), std::end(lhs), rhs, rhs + Traits::length(rhs));
    }

    // operator <=
    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator<=(const basic_string<CharT, MaxSize, Traits>& lhs, const basic_string<CharT, MaxSize, Traits>& rhs)
    {
        return !(rhs < lhs);
    }

    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator<=(const CharT* lhs, const basic_string<CharT, MaxSize, Traits>& rhs)
    {
        return !(rhs < lhs);
    }

    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator<=(const basic_string<CharT, MaxSize, Traits>& lhs, const CharT* rhs)
    {
        return !(rhs < lhs);
    }

    // operator >
    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator>(const basic_string<CharT, MaxSize, Traits>& lhs, const basic_string<CharT, MaxSize, Traits>& rhs)
    {
        return rhs < lhs;
    }

    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator>(const CharT* lhs, const basic_string<CharT, MaxSize, Traits>& rhs)
    {
        return rhs < lhs;
    }

    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator>(const basic_string<CharT, MaxSize, Traits>& lhs, const CharT* rhs)
    {
        return rhs < lhs;
    }

    // operator >=
    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator>=(const basic_string<CharT, MaxSize, Traits>& lhs, const basic_string<CharT, MaxSize, Traits>& rhs)
    {
        return !(lhs < rhs);
    }

    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator>=(const CharT* lhs, const basic_string<CharT, MaxSize, Traits>& rhs)
    {
        return !(lhs < rhs);
    }

    template <typename CharT, size_t MaxSize, typename Traits>
    constexpr bool operator>=(const basic_string<CharT, MaxSize, Traits>& lhs, const CharT* rhs)
    {
        return !(lhs < rhs);
    }

    // operator <<
    template <typename CharT, size_t MaxSize, typename Traits>
    std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const basic_string<CharT, MaxSize, Traits>& str)
    {
        return os << str.data();
    }

    // operator >>

    template <typename CharT, size_t MaxSize, typename Traits>
    std::basic_string<CharT, Traits> to_string(const basic_string<CharT, MaxSize, Traits>& str)
    {
        return { str.data(), str.size() };
    }

    template <size_t MaxSize>
    using string = basic_string<char, MaxSize>;

    using string32 = string<32>;
    using string64 = string<64>;
    using string128 = string<128>;
    using string256 = string<256>;
	
} // namespace native
