#include "HorizontalVertexArray.h"

HorizontalVertexArray::HorizontalVertexArray()
:is_falling(false), is_main(false){
    vec.resize(const_screen_width);
}

HorizontalVertexArray::~HorizontalVertexArray() = default;

void HorizontalVertexArray::push(const int &position, VerticalVertexArray* new_vec) {
    vec.at(position) = new_vec;
}

void HorizontalVertexArray::pop(const int &position) {

}

void HorizontalVertexArray::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for(auto& vva : vec){
        if(vva != nullptr)
            vva->draw(target, states);
    }
}

VerticalVertexArray* HorizontalVertexArray::at(const int &position) {
    //if(position >= vec.size()) return nullptr;
    return vec.at(position);
}

void HorizontalVertexArray::prepare_for_drawing() {
    drawer.clear();
    for(auto& vva : vec){
        for(auto& ver : *vva->get_vector()){
            drawer.append(ver);
        }
    }
}

bool HorizontalVertexArray::contains_vertex_at(const sf::Vector2f &position) {
    auto vva = at(position.x);
    if(vva != nullptr){
        if(position.y >= vva->top() && position.y <= vva->bottom())
            return true;
    }
    return false;
}

bool HorizontalVertexArray::contains_vertex_between(const sf::Vector2f &bottom, const sf::Vector2f &top) {
    auto vva = at(bottom.x);
    if(vva != nullptr) {
        if((vva->bottom() > top.y && vva->bottom() < bottom.y) || (vva->top() > top.y && vva->top() < bottom.y)){
            return true;
        }
    }
    return false;
}

void HorizontalVertexArray::tidy() {
    for(auto & i : vec){
        if(i != nullptr && i->count() == 0){
            i = nullptr;
            //adelete i;
        }
    }
}

int HorizontalVertexArray::count() {
    int cnt = 0;
    for(auto& vva : vec){
        if (vva != nullptr && vva->count() > 0) cnt++;
    }
    return cnt;
}

void HorizontalVertexArray::move(const int &num_pixels, HorizontalVertexArray &array, std::string direction) {
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

void HorizontalVertexArray::push(HorizontalVertexArray &array) {
//    for(int i = 0; i < vec.size(); i++){
//        if(vec.at(i) != nullptr){
//            array.at(i)->push(array.at(i)->top() - vec.at(i)->top());
//            vec.at(i) = nullptr;
//        }
//    }
}
