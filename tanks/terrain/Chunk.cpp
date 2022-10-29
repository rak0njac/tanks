#include "Chunk.h"

Chunk::Chunk()
        :is_falling(false){
    vec.resize(const_screen_width);
}

Chunk::~Chunk() = default;

void Chunk::push(const int &position, VerticalLine* new_vec) {
    vec.at(position) = new_vec;
}

void Chunk::pop(const int &position) {

}

void Chunk::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for(auto& vva : vec){
        if(vva != nullptr){
            vva->draw(target, states);
        }
    }
}

VerticalLine* Chunk::at(const int &position) {
    //if(position >= vec.size()) return nullptr;
    return vec.at(position);
}

bool Chunk::contains_vertex_at(const sf::Vector2f &position) {
    auto vva = at(position.x);
    if(vva != nullptr && vva->count() > 0){
        if(position.y >= vva->top() && position.y <= vva->bottom())
            return true;
    }
    return false;
}

//bool legacy_HorizontalVertexArray::is_between(const sf::Vector2f &bottom, const sf::Vector2f &top) {
//    if(bottom.x != top.x) return false;
//    auto vva = at(bottom.x);
//    if(vva != nullptr) {
//        if(vva->bottom() < bottom.y && vva->top() > top.y){
//            return true;
//        }
//    }
//    return false;
//}

void Chunk::tidy() {
    int index = 0;
    for(auto & i : vec){
        if(i != nullptr && i->count() == 0){
            //std::cout << "Tidier found empty NVL at " << index << " and is thus declaring it nullptr and deleting." << std::endl;
            i = nullptr;
            delete i;
        }
        index++;
    }
}

int Chunk::count() {
    int cnt = 0;
    for(auto& vva : vec){
        if (vva != nullptr && vva->count() > 0) cnt++;
    }
    return cnt;
}

void Chunk::move(const int &num_pixels, Chunk &array, std::string direction) {
    int cnt = 0;
    for(int i = 0; i < vec.size(); i++){
        if(vec.at(i) != nullptr && vec.at(i)->count()> 0){
            if(vec.at(i)->bottom() < array.at(i)->top()){
                vec.at(i)->move(num_pixels, direction);
                cnt++;
            }
            else{
                array.at(i)->push(*vec.at(i)->get_vector()); //TODO: Use this
                //std::cout << "NVL at " << i << " pushed to main terrain and cleared. ";
                vec.at(i) = nullptr;
                delete vec.at(i);
                //std::cout << "Deleted!" << std::endl;
            }
        }
    }
    if(cnt == 0){
        is_falling = false;
    }
}

void Chunk::push(Chunk &array) {
//    for(int i = 0; i < vec.size(); i++){
//        if(vec.at(i) != nullptr){
//            array.at(i)->push(array.at(i)->top() - vec.at(i)->top());
//            vec.at(i) = nullptr;
//        }
//    }
}
