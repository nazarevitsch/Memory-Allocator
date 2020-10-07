#include <iostream>
using namespace std;

struct Header{
    uint8_t used;
    uint16_t size;
    uint16_t previous_size;
};

class Allocator{
public:
    uint8_t* start;
    size_t size;
    Allocator();
    void init(size_t size);
    void* mem_alloc(std::size_t size);
    void* mem_realloc(void* adr, size_t size);
    void mem_free(void *adr);
    void create_header(void* start, uint8_t used, uint16_t size, uint16_t previous_size);
    void mem_dump();
    void unite(void* adr);
    size_t align(size_t size);
};

Allocator::Allocator() {
}

void Allocator::create_header(void* start, uint8_t used, uint16_t size, uint16_t previous_size){
    Header* header = (Header*) start;
    header->used = used;
    header->size = size;
    header->previous_size = previous_size;
}

void Allocator::init(size_t size) {
    size_t size_buffer = sizeof(char) * size;
    start = (uint8_t *)malloc(size_buffer);
    this->size = size;
    create_header(start, 0, size - 8, 0);
}

void* Allocator::mem_alloc(size_t size) {
    Header *header = (Header *) start;
    size_t handle = 8 + header->size;
    size_t previous_size = 0;
    size_t current_size = header->size;
    while (true) {
        if ((int) header->used == 0 && header->size - 8 - 4 >= align(size)) {
            create_header(header, 1, align(size), previous_size);
            uint8_t* answer = (uint8_t*) header;
            create_header(answer + 8 + align(size), 0, current_size - 8 - align(size), align(size));
            return (uint8_t**)(answer + 8);
        } else {
            if (handle < this->size) {
                previous_size = header->size;
                uint8_t *answer = (uint8_t *) header;
                header = (Header *) (answer + 8 + (int) header->size);
                current_size = header->size;
                handle = handle + 8 + header->size;
                if (handle > this->size) return NULL;
            } else return NULL;
        }
    }
}

void* Allocator::mem_realloc(void* adr, size_t size){
    if (adr == NULL){
        return mem_alloc(size);
    } else{
        void* answer = mem_alloc(size);
        if (answer != NULL){
            mem_free(adr);
        }
        return answer;
    }
}

void Allocator::mem_free(void *adr) {
    uint8_t* start = (uint8_t*)adr;
    Header* header = (Header*)(start - 8);
    header->used = 0;
    unite(adr);
}

void Allocator::mem_dump() {
    Header *header = (Header *) start;
    size_t handle = 8 + header->size;
    while (true) {
        if (handle <= this->size) {
            cout << "HEADER Size:  " << 8 << endl;
            cout << "HEADER USED:  " << (int)header->used << endl;
            cout << "MEMORY SIZE OF THIS BLOCK:  " << header->size << endl;
            cout << "MEMORY SIZE OF PREVIOUS BLOCK: " << header->previous_size << endl;
            uint8_t* pp = (uint8_t*)header;
            for (size_t i = 0; i < 8; i++) {
                cout << "I: " << i << ",  P: " << (uint8_t **) (pp + i) << endl;
            }
            cout << "MEMORY SIZE:  " << header->size << endl;
            for (size_t i = 0; i < header->size; i++) {
                cout << "I: " << i << ",  P: " << (uint8_t **) (pp + 8 + i) << endl;
            }
            if (handle == this->size){
                break;
            } else {
                uint8_t* p = (uint8_t*)header;
                header = (Header*)(p + 8 + header->size);
                handle = handle + 8 + header->size;
            }
        } else break;
    }
}

void Allocator::unite(void* adr) {
    cout << adr << endl;
    uint8_t *start = (uint8_t *) adr;
    Header *current_header = (Header *) (start - 8);
    cout << "C.H:  " << current_header << endl;
    Header *previous_header = NULL;
    Header *next_header = NULL;
    if (current_header > (Header *) (uint8_t **) this->start) {
        previous_header = (Header *) (start - 16 - current_header->previous_size);
        if (previous_header->used == 0) {
            previous_header->size = previous_header->size + 8 + current_header->size;
            current_header = previous_header;
        }
    }
    next_header = (Header*)(((uint8_t *) current_header) + current_header->size + 8);
    if (next_header->used == 0) {
        current_header->size = current_header->size + 8 + next_header->size;
    }
}

size_t Allocator::align(size_t size) {
    if (size % 4 == 0){
        return size;
    } else{
        return size + (4 - (size % 4));
    }
}


int main() {
    Allocator allocator;
    allocator.init(100);
    void* adr1 = allocator.mem_alloc(10);
    void* adr2 = allocator.mem_alloc(10);
    void* adr3 = allocator.mem_alloc(10);
    allocator.mem_free(adr2);
    allocator.mem_free(adr3);
    allocator.mem_dump();
    return 0;
}