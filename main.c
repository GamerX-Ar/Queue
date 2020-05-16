/**
 * Пример реализации циклической очереди на массиве.
 * Другие способы реализации - на связном списке или на двух стеках.
 * В данной реализации одна ячейка из n всегда будет незанятой
 * (так как очередь с n элементами невозможно отличить от пустой).
 */
#include <stdio.h>
#include <stdlib.h>

#define QUEUE_OVERFLOW  -100
#define QUEUE_EMPTY -101
#define OUT_OF_MEMORY   -102

#define QUEUE_SIZE 8

struct queue_t;

typedef struct queue_t queue_t;
typedef char T;


struct queue_t {
    T *_array;  // массив, на основе которого реализуется очередь
    short _first;  // начало очереди
    short _last;  // место в очереди, на которое встанет новый элемент
    unsigned char _max_length;  // максимальный размер очереди

    short (*length)(queue_t * const);  // текущий размер очереди
    T (*peek)(queue_t * const);  // элемент в начале очереди
    void (*enqueue)(const T, queue_t * const);  // поставить в очередь
    T (*dequeue)(queue_t * const);  // убрать из очереди
    void (*print)(queue_t * const);  // распечатать очередь

    void (*destroy)(queue_t *);  // освободить память
};

/**
 * Освободить динамически выделенную память.
 *
 * @param this Указатель на инстанс структуры
 */
void destroy(queue_t *this) {
    free(this->_array);
}

/**
 * Распечатать очередь.
 *
 * @param this Указатель на инстанс структуры
 */
void print(queue_t * const this) {
    for (int i = this->_first; i != this->_last; i == this->_max_length - 1 ? i = 0 : ++i) {
        printf("%c ", this->_array[i]);
    }
}

/**
 * Убрать из очереди.
 *
 * @param this Указатель на инстанс структуры
 * @return Вышедший элемент очереди
 */
T dequeue(queue_t * const this) {
    if (this->_first == this->_last) {
        printf("Queue empty!\n");
        return 0;  // ASCII code 0 maps to NUL
    }
    return this->_array[this->_first == this->_max_length ? this->_first = 0 : this->_first++];
}

/**
 * Поставить в очередь.
 *
 * @param item
 * @param this Указатель на инстанс структуры
 */
void enqueue(const T item, queue_t * const this) {
    // Очередь переполнена, когда last на единицу меньше first, либо
    // когда last указывает на конец массива, a first - на начало.
    if ( (this->_last + 1 == this->_first) || (this->_last == this->_max_length && !this->_first) ) {
        printf("Queue overflow! Item '%c' is lost.\n", item);
        return;
    }
    this->_array[this->_last++] = item; // сначала ставим элемент на место last, затем увеличиваем на единицу
    if (this->_last == this->_max_length) this->_last = 0; // зацикливание очереди
}

/**
 * Возвращает элемент, находящийся в начале очереди, без его удаления.
 *
 * @param this Указатель на инстанс структуры
 * @return Первый элемент в очереди
 */
T peek(queue_t * const this) {
    if (this->_first == this->_last) {
        printf("Queue empty!\n");
        return 0;  // ASCII code 0 maps to NUL
    }
    return this->_array[this->_first];
}

/**
 * Текущий размер очереди.
 *
 * @param this
 * @return Текущая длина очереди
 */
short length(queue_t * const this) {
    return this->_last;
}

/**
 * Инициализация очереди.
 *
 * @param stack Указатель на управляющую структуру
 * @param size Максимально возможная длина очереди
 */
void init_queue(queue_t *stack, unsigned char size) {
    stack->_array = malloc(size * sizeof(T));
    if (stack->_array == NULL) {
        printf("Out of memory!\n");
        exit(OUT_OF_MEMORY);
    }
    stack->_first = 0;
    stack->_last = 0;
    stack->_max_length = size;
    stack->length = &length;
    stack->peek = &peek;
    stack->enqueue = &enqueue;
    stack->dequeue = &dequeue;
    stack->print = &print;
    stack->destroy = &destroy;
}

int main() {

    queue_t queue;

    init_queue(&queue, QUEUE_SIZE);

    queue.enqueue('f', &queue);
    queue.enqueue('i', &queue);
    queue.enqueue('r', &queue);
    queue.enqueue('s', &queue);
    queue.enqueue('t', &queue);

    queue.print(&queue); printf("\n");

    queue.dequeue(&queue);
    queue.dequeue(&queue);

    queue.print(&queue); printf("\n");

    queue.enqueue('a', &queue);
    queue.enqueue('b', &queue);

    queue.print(&queue); printf("\n");

    queue.dequeue(&queue);

    queue.print(&queue); printf("\n");

    queue.enqueue('l', &queue);
    queue.enqueue('a', &queue);
    queue.enqueue('s', &queue);
    queue.enqueue('t', &queue); // queue overflow

    queue.print(&queue); printf("\n");

    queue.destroy(&queue);

    return 0;
}