#pragma once


#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <algorithm>
#include <iterator>
using namespace std;



template <typename Type>
class SingleLinkedList {
    // Узел списк

    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

public:
    SingleLinkedList() : size_(0) {}
    SingleLinkedList(std::initializer_list<Type> values)
    {
        SingleLinkedList<Type> temp;
        for (auto i = values.begin(); i != values.end(); i++)
        {
            temp.PushBack(*i);
        }
        swap(temp);
    }
    SingleLinkedList(const SingleLinkedList& other)
    {
        // Сначала надо удостовериться, что текущий список пуст
        SingleLinkedList tmp;
        for (auto i = other.begin(); i != other.end(); i++)
        {
            tmp.PushBack(*i);
        }
        // После того как элементы скопированы, обмениваем данные текущего списка и tmp
        swap(tmp);
        // Теперь tmp пуст, а текущий список содержит копию элементов other
    }
    ~SingleLinkedList() {
        Clear();
    }

    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }

    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
    void PushBack(const Type& value)
    {
        Node* new_node = new Node(value, nullptr);
        if (head_.next_node == nullptr) {
            head_.next_node = new_node;
        }
        else {
            Node* last_node = head_.next_node;
            while (last_node->next_node != nullptr) {
                last_node = last_node->next_node;
            }
            last_node->next_node = new_node;
        }
        size_++;
    }

    void Clear() noexcept
    {
        while (head_.next_node != nullptr) {
            auto temp = head_.next_node;
            head_.next_node = temp->next_node;
            delete temp;
            size_--;
        }
    }
    void PopFront()
    {
        auto temp = head_.next_node;
        head_.next_node = temp->next_node;
        delete temp;
        size_--;
    }

    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        return (size_ == 0);
    }

    void swap(SingleLinkedList& other) noexcept
    {
        // Реализуйте обмен содержимого списков самостоятельно
        auto ptr = other.head_.next_node;
        other.head_.next_node = head_.next_node;
        head_.next_node = ptr;

        auto Size = other.size_;
        other.size_ = size_;
        size_ = Size;
    }

    SingleLinkedList& operator=(const SingleLinkedList& rhs)
    {
        // Реализуйте присваивание самостоятельно
        SingleLinkedList tmp;
        for (auto i = rhs.begin(); i != rhs.end(); i++)
        {
            tmp.PushBack(*i);
        }
        if (size_ > 0)
        {
            Clear();
        }
        // После того как элементы скопированы, обмениваем данные текущего списка и tmp
        swap(tmp);
        return *this;
    }

    template <typename ValueType>
    class BasicIterator {
        // Класс списка объявляется дружественным, чтобы из методов списка
        // был доступ к приватной области итератора
        friend class SingleLinkedList;

        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node) {
            node_ = node;
            // Реализуйте конструктор самостоятельно
        }

    public:
        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора

        // Категория итератора — forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
        // Тип указателя на итерируемое значение
        using pointer = ValueType*;
        // Тип ссылки на итерируемое значение
        using reference = ValueType&;

        BasicIterator() = default;

        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept {
            this->node_ = other.node_;
        }

        // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return this->node_ != rhs.node_;
        }

        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ != rhs.node_;
        }

        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept {
            this->node_ = node_->next_node;
            return *this;
        }

        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept {
            auto* old = node_;
            this->node_ = node_->next_node;
            return BasicIterator(old);
        }

        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept {
            return (*node_).value;
        }

        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept {
            return &(*node_).value;
        }

    private:
        Node* node_ = nullptr;
    };


public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;

    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;

    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept {
        return Iterator(head_.next_node);
    }

    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept {
        return Iterator(nullptr);
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        if (head_.next_node == nullptr) {
            return ConstIterator(nullptr);
        }
        return ConstIterator(head_.next_node);
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        return ConstIterator(nullptr);
    }

    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        if (head_.next_node == nullptr) {
            return ConstIterator(nullptr);
        }
        return ConstIterator(head_.next_node);
    }

    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept {
        return ConstIterator(nullptr);
    }
    // Возвращает итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator before_begin() noexcept
    {
        return Iterator{ &head_ };
    }
    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
        // Реализуйте самостоятельно
        return ConstIterator{ const_cast<Node*>(&head_) };
    }
    // Возвращает константный итератор, указывающий на позицию перед первым элементом односвязного списка.
    // Разыменовывать этот итератор нельзя - попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator before_begin() const noexcept {
        // Реализуйте самостоятельно
        return ConstIterator{ const_cast<Node*>(&head_) };
    }
    Iterator InsertAfter(ConstIterator pos, const Type& value) {
        Node* node = new Node(value, pos.node_->next_node);
        pos.node_->next_node = node;
        ++size_;
        return Iterator{ pos.node_->next_node };
    }
    Iterator EraseAfter(ConstIterator pos) noexcept
    {
        if (pos.node_->next_node != nullptr)
        {
            auto to_delete = pos.node_->next_node;
            pos.node_->next_node = to_delete->next_node;
            delete to_delete;
            --size_;
        }
        return Iterator{ pos.node_->next_node };
    }

private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_;
};


template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept
{
    lhs.swap(rhs);
}
template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return equal(lhs.begin(), lhs.end(), rhs.begin());
}
template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    //return equal(lhs.begin(), lhs.end(), rhs.begin());
    return !(lhs == rhs);
}
template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return lexicographical_compare(lhs.begin(), lhs.end(),
        rhs.begin(), rhs.end());
}
template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return !(lhs < rhs);
}
template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return ((lhs < rhs) || (lhs == rhs));
}
template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs)
{
    return ((lhs > rhs) || (lhs == rhs));
}
