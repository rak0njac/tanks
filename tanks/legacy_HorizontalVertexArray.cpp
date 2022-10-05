#include "legacy_HorizontalVertexArray.h"

legacy_HorizontalVertexArray::legacy_HorizontalVertexArray()
:is_falling(false){
    vec.resize(const_screen_width);
}

legacy_HorizontalVertexArray::~legacy_HorizontalVertexArray() = default;

void legacy_HorizontalVertexArray::push(const int &position, legacy_VerticalVertexArray* new_vec) {
    vec.at(position) = new_vec;
}

void legacy_HorizontalVertexArray::pop(const int &position) {

}

void legacy_HorizontalVertexArray::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for(auto& vva : vec){
        if(vva != nullptr)
            vva->draw(target, states);
    }
}

legacy_VerticalVertexArray* legacy_HorizontalVertexArray::at(const int &position) {
    //if(position >= vec.size()) return nullptr;
    return vec.at(position);
}

bool legacy_HorizontalVertexArray::contains_vertex_at(const sf::Vector2f &position) {
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

void legacy_HorizontalVertexArray::tidy() {
    for(auto & i : vec){
        if(i != nullptr && i->count() == 0){
            i = nullptr;
            delete i;
        }
    }
}

int legacy_HorizontalVertexArray::count() {
    int cnt = 0;
    for(auto& vva : vec){
        if (vva != nullptr && vva->count() > 0) cnt++;
    }
    return cnt;
}

void legacy_HorizontalVertexArray::move(const int &num_pixels, legacy_HorizontalVertexArray &array, std::string direction) {
    int cnt = 0;
    //std::cout << "Count: " << cnt << std::endl;
    for(int i = 0; i < vec.size(); i++){
        if(vec.at(i) != nullptr && vec.at(i)->count()> 0){
            if(vec.at(i)->bottom() < array.at(i)->top()){
                vec.at(i)->move(num_pixels, direction);
                cnt++;
            }
            else{
                //std::cout << "Main terrain top: " << array.at(i)->top() << std::endl;
                //std::cout << "Falling terrain top: " << vec.at(i)->top() << std::endl;
                array.at(i)->push(array.at(i)->top() - vec.at(i)->top());
                vec.at(i)->clear();
            }
        }
    }
    //std::cout << std::endl;
    if(cnt == 0){
        is_falling = false;
    }
}

void legacy_HorizontalVertexArray::push(legacy_HorizontalVertexArray &array) {
//    for(int i = 0; i < vec.size(); i++){
//        if(vec.at(i) != nullptr){
//            array.at(i)->push(array.at(i)->top() - vec.at(i)->top());
//            vec.at(i) = nullptr;
//        }
//    }
}
