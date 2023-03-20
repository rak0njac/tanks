#include "TerrainEngine.h"

TerrainEngine::TerrainEngine()= default;
TerrainEngine::~TerrainEngine() = default;

std::vector<std::vector<sf::Vertex>> TerrainEngine::generate_half_circle(int m_radius, int m_pos_x, int m_pos_y) {
    int segments = m_radius * 7;     //radius * 7 should be enough segments for a result without holes for any radius size
    float step = 2 * const_pi / segments;
    float angle = 0.0f;
    std::vector<sf::Vertex> upper, lower;

    for (int i = 0; i < segments; ++i, angle += step) {
        int vertX = std::round(m_pos_x + cosf(angle) * m_radius);
        if(vertX < 0 || vertX >= const_screen_width)
            continue;
        int vertY = std::round(m_pos_y + sinf(angle) * m_radius);
        auto vert = sf::Vertex(sf::Vector2f(vertX, vertY));
        if(angle < const_pi){
            vert.color = sf::Color::Red;
            lower.emplace_back(vert);
        }
        else{
            vert.color = sf::Color::Green;
            upper.emplace_back(vert);
        }
    }
    return std::vector<std::vector<sf::Vertex>> {upper, lower};
}

void TerrainEngine::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.texture = &texture;


    if(const_optimized_for == "GPU"){
        target.draw(va, states);
    }
    else{
        main_terrain.draw(target, states);
        for (auto t : falling_terrains) {
            //t.set_color(sf::Color::Red);
            t.draw(target, states);
            //for (auto& v : t.vec) {
            //    if (v != nullptr) {
            //        auto vec = v->get_vector();
            //        for (auto& vertex : *vec) {
            //            sf::CircleShape circle(1, 5);
            //            circle.setPosition(vertex.position);
            //            target.draw(circle, states);
            //        }
            //    }
            //}
        }
        target.draw(debug_boundaries);
    }
    //for (auto& v : main_terrain.vec) {
    //    auto vec = v->get_vector();
    //    for (auto& vertex : *vec) {
    //        sf::CircleShape circle(1, 5);
    //        circle.setPosition(vertex.position);
    //        target.draw(circle, states);
    //    }
    //}
}

TerrainEngine::TerrainEngine(const std::string& texture_filename)
{
	texture.loadFromFile(texture_filename);
	texture.setRepeated(true);

	randomize(20); //ideally between 10 and 50

    if(const_debug_enabled){
        Chunk hva;
        for(int i = 50; i < 200; i++){
            VerticalLine* vva = new VerticalLine(i, 150, 20);
            hva.push(i, vva);
        }
        falling_terrains.emplace_back(hva);
    }

    va.resize(const_screen_width * const_screen_height);
    va.setPrimitiveType(sf::PrimitiveType::Lines);
}

void TerrainEngine::destroy_circle(sf::Vector2i pos_projectile, int radius) {
    auto half_circle = generate_half_circle(radius, pos_projectile.x, pos_projectile.y);
    if(const_debug_enabled){
        for(auto& i : half_circle){
            for(auto& j : i){
                debug_boundaries.append(j);
            }
        }
    }

    auto& lower = half_circle.at(1);

    Chunk hva_upper;
    tidy();

    for(int i = 0, cur = 0; i < lower.size(); i++){
        auto& pos_lower = lower.at(i).position; //position of current lower half circle vertex
        auto pos_upper = sf::Vector2f(pos_lower.x, pos_projectile.y - (pos_lower.y - pos_projectile.y)); //position of current upper half circle vertex

        //do not operate on same horizontal pixel multiple times
        if(pos_lower.x == cur)
            continue;
        cur = pos_lower.x;

        if(pos_upper.y >= const_screen_height){
            pos_upper.y = const_screen_height - 1;
        }

        int index = 0;
            //std::cout << "Number of falling terrain chunks: " << falling_terrains.size() << std::endl;
        //iterate through all terrain chunks
        destroy_circle(pos_lower, pos_upper, main_terrain, hva_upper);
        for(auto& j : falling_terrains){
            if(const_debug_enabled){
                //std::cout << std::endl;
                //std::cout << "Currently operating on terrain chunk at index: " << index << " for position " << pos_lower.x << std::endl;
            }
            destroy_circle(pos_lower, pos_upper, j, hva_upper);
            index++;
        }
        if(const_debug_enabled) {}
            //std::cout << "Currently operating on main terrain for position " << pos_lower.x << std::endl;

    }
    hva_upper.is_falling = true;
    if(hva_upper.count() > 0)
        falling_terrains.emplace_back(hva_upper);

    //std::cout << falling_terrains.size() << std::endl;

    //for(int i = 0; i < const_screen_width; i++){
    //    for(int j = 0; j < falling_terrains.size(); j++){
    //        std::cout << std::endl;
    //        std::cout << "************** REPORT :: terrain chunk at index " << j << std::endl;
    //        std::cout << std::endl;
    //        if(falling_terrains.at(j).at(i) == nullptr){
    //            std::cout << "************** REPORT :: x position " << i << " is nullptr" << std::endl;
    //        }
    //        else{
    //            for(auto& v : *falling_terrains.at(j).at(i)->get_vector()){
    //                std::cout << "************** REPORT :: vertex x position: " << v.position.x << std::endl;
    //                std::cout << "************** REPORT :: vertex y position: " << v.position.y << std::endl;
    //            }
    //        }
    //    }

    //    std::cout << std::endl;
    //    //std::cout << "************** REPORT :: main terrain" << std::endl;
    //    //std::cout << std::endl;
    //    //if(main_terrain.at(i) == nullptr){
    //    //    std::cout << "************** REPORT :: x position " << i << " is nullptr" << std::endl;
    //    //}
    //    //else{
    //    //    for(auto& v : *main_terrain.at(i)->get_vector()){
    //    //        std::cout << "************** REPORT :: vertex x position: " << v.position.x << std::endl;
    //    //        std::cout << "************** REPORT :: vertex y position: " << v.position.y << std::endl;
    //    //    }
    //    //}
    //}
}

//TODO clean this shit!!! learn about inline functions
void TerrainEngine::destroy_circle(sf::Vector2f& pos_lower, sf::Vector2f& pos_upper, Chunk& terrain_chunk, Chunk& new_terrain_chunk_upper) {
    auto vva = terrain_chunk.at(pos_lower.x);
    if(vva != nullptr){
        if(vva->contains_vertex_at(pos_upper.y)){
            VerticalLine* new_vva_upper = new VerticalLine(pos_upper.y, vva);

            if(new_vva_upper->count() > 0){
                new_terrain_chunk_upper.push(pos_upper.x, new_vva_upper);
                if(vva->contains_vertex_at(pos_lower.y)){
                    vva->pop(pos_lower.y - vva->top());
                }
                else{
                    vva->clear();
                }
            }
            else{
                delete new_vva_upper;
            }
        }

        //if it only has a vertex at lower half circle position, remove the right number of pixels from the current iteration's terrain chunk
        else if(vva->contains_vertex_at(pos_lower.y)){
            vva->pop(pos_lower.y - vva->top());
        }

        //if it doesn't have neither upper half nor lower half circle position vertices, but has some inbetween, delete all of those
        if(vva->is_between(pos_lower.y, pos_upper.y)){
            vva->clear();
        }
    }
}

void TerrainEngine::randomize(int roughness) {
    bool generating = true;
    int midpoint = const_screen_height / 2 + rnd::get_int(const_screen_height / -3, const_screen_height / 3);   //height-based midpoint off of which the next vertex' height should be decided
    if (roughness > midpoint){
        std::cout << "TANKS ERROR 0x00000000: Drawing limit exceeded. Try lowering roughness." << std::endl;
        return;
    }
    sf::Vector2i prev;
    sf::Vector2i next = sf::Vector2i(0, midpoint);

    Chunk hva;

    while(generating){
        prev = next;
        next.x += rnd::get_int(10, 50);
        if(next.x > const_screen_width) {
            next.x = const_screen_width;
            generating = false;
        };
        midpoint = next.y;
        int displacement = rnd::get_int(-roughness, roughness);

        //repeat random generation until sure next.y is near midpoint and not near the screen limits
        while(  next.y + displacement > midpoint + roughness ||
                next.y + displacement < midpoint - roughness ||
                next.y + displacement > const_screen_height - const_screen_height / 6 ||
                next.y + displacement < const_screen_height / 6)
        {
            displacement = rnd::get_int(-roughness, roughness);
        }
        next.y += displacement;

        //linear function algorithm for calculating y to populate the debug_boundaries vertex vector where x moves by 1
        int x1 = prev.x;
        int x2 = next.x;
        float y1 = prev.y;
        float y2 = next.y;

        float step = (y2 - y1)/(x2 - x1);

        for(int j = 0; j < (x2 - x1); j++){
            y1 += step;
            sf::Vector2f ver(x1 + j, y1);
            hva.push(x1 + j, new VerticalLine(x1 + j, const_screen_height - 5, y1));
        }
    }
    main_terrain = hva;
}

void TerrainEngine::logic() {
    if(const_optimized_for == "GPU") {
        va.clear();
        for(auto& hva : falling_terrains){
            if(hva.is_falling){
                hva.move(1, main_terrain);  //fall by one pixel until it hits the main terrain chunk
            }
            for(int i = 0; i < const_screen_width; i++){
                if(hva.at(i) != nullptr){
                    for(auto& vertex : *hva.at(i)->get_vector()){
                        va.append(vertex);
                    }
                }
            }
        }

        for(int i = 0; i < const_screen_width; i++){
            if(main_terrain.at(i) != nullptr){
                for(auto& vertex : *main_terrain.at(i)->get_vector()){
                    va.append(vertex);
                }
            }
        }
    }
    else{
        for(auto& hva : falling_terrains){
            if(hva.is_falling){
                hva.move(1, main_terrain);  //fall by one pixel until it hits the main terrain chunk
            }
        }
    }

}

Chunk* TerrainEngine::get_main_terrain() {
    return &main_terrain;
}

void TerrainEngine::tidy() {
    int cnt = 0;
    std::vector<Chunk> new_falling_terrains;
    for(auto & falling_terrain : falling_terrains){
        falling_terrain.tidy();
        //std::cout << "TerrainEngine tidier finished, current FT count is: " << falling_terrain.count() << std::endl;
        if(falling_terrain.count() == 0){
            cnt++;
        }
        else{
            new_falling_terrains.emplace_back(falling_terrain);
        }
    }
    falling_terrains = new_falling_terrains;
}

std::vector<Chunk> TerrainEngine::get_all_terrains() {
    //TODO: optimize
    std::vector<Chunk> vec = falling_terrains;
    vec.emplace_back(main_terrain);
    return vec;
}
