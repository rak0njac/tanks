#include "TerrainChunk.h"

TerrainChunk::TerrainChunk()
        :is_falling(false){
    vec.resize(const_screen_width);
}

TerrainChunk::~TerrainChunk() = default;

void TerrainChunk::push(const int &position, NewVerticalLine* new_vec) {
    vec.at(position) = new_vec;
}

void TerrainChunk::pop(const int &position) {

}

void TerrainChunk::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    for(auto& vva : vec){
        if(vva != nullptr){
            vva->draw(target, states);
        }
    }
}

NewVerticalLine* TerrainChunk::at(const int &position) {
    //if(position >= vec.size()) return nullptr;
    return vec.at(position);
}

bool TerrainChunk::contains_vertex_at(const sf::Vector2f &position) {
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

void TerrainChunk::tidy() {
    for(auto & i : vec){
        if(i != nullptr && i->count() == 0){
            i = nullptr;
            delete i;
        }
    }
}

int TerrainChunk::count() {
    int cnt = 0;
    for(auto& vva : vec){
        if (vva != nullptr && vva->count() > 0) cnt++;
    }
    return cnt;
}

void TerrainChunk::move(const int &num_pixels, TerrainChunk &array, std::string direction) {
    int cnt = 0;
    for(int i = 0; i < vec.size(); i++){
        if(vec.at(i) != nullptr && vec.at(i)->count()> 0){
            if(vec.at(i)->bottom() < array.at(i)->top()){
                vec.at(i)->move(num_pixels, direction);
                cnt++;
            }
            else{
                array.at(i)->push(array.at(i)->top() - vec.at(i)->top());
                //array.at(i)->push(*vec.at(i)->get_vector()->at(0)); //TODO: Use this
                vec.at(i)->clear();
            }
        }
    }
    if(cnt == 0){
        is_falling = false;
    }
}

void TerrainChunk::push(TerrainChunk &array) {
//    for(int i = 0; i < vec.size(); i++){
//        if(vec.at(i) != nullptr){
//            array.at(i)->push(array.at(i)->top() - vec.at(i)->top());
//            vec.at(i) = nullptr;
//        }
//    }
}
