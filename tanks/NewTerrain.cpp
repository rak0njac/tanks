#include "NewTerrain.h"

NewTerrain::NewTerrain()= default;
NewTerrain::~NewTerrain() = default;

std::vector<std::vector<sf::Vertex>> NewTerrain::generate_half_circle(int m_radius, int m_pos_x, int m_pos_y) {
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

void NewTerrain::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	states.texture = &texture;
    main_terrain.draw(target, states);
    for(auto& t : falling_terrains)
        t.draw(target, states);
    target.draw(debug_boundaries);
}

NewTerrain::NewTerrain(const std::string& texture_filename)
{
	texture.loadFromFile(texture_filename);
	texture.setRepeated(true);

	randomize(20); //ideally between 10 and 50
}

void NewTerrain::destroy_circle(sf::Vector2i pos_projectile, int radius) {
    auto half_circle = generate_half_circle(radius, pos_projectile.x, pos_projectile.y);
    if(const_debug_enabled){
        for(auto& i : half_circle){
            for(auto& j : i){
                debug_boundaries.append(j);
            }
        }
    }

    auto& lower = half_circle.at(1);
    std::reverse(lower.begin(), lower.end());

    HorizontalVertexArray hva_upper;
    HorizontalVertexArray hva_lower;

    for(int i = 0, cur = 0; i < lower.size(); i++){
        auto& pos_lower = lower.at(i).position;
        auto pos_upper = sf::Vector2f(pos_lower.x, pos_projectile.y - (pos_lower.y - pos_projectile.y));// upper.at(i).position;
        if(pos_lower.x == cur)
            continue;
        cur = pos_lower.x;

        destroy_circle(pos_lower, pos_upper, main_terrain, hva_upper, hva_lower);

        for(auto& j : falling_terrains){
            destroy_circle(pos_lower, pos_upper, j, hva_upper, hva_lower);
        }

    }

    tidy();

        hva_upper.is_falling = true;
        hva_lower.is_falling = true;
    //falling_terrains.pop_back();
    falling_terrains.emplace_back(hva_upper);
    falling_terrains.emplace_back(hva_lower);
}

//TODO clean this shit!!! learn about inline functions
void NewTerrain::destroy_circle(sf::Vector2f& pos_lower, sf::Vector2f& pos_upper, HorizontalVertexArray& j, HorizontalVertexArray& hva_upper, HorizontalVertexArray& hva_lower) {
    auto v = j.at(pos_lower.x);
    if(v != nullptr){
        if(j.contains_vertex_at(pos_upper)){
            VerticalVertexArray* vva = new VerticalVertexArray(pos_upper.x, pos_upper.y, v->top() );
            vva->set_color(sf::Color::Red);
            if(vva->count() > 0)
                hva_upper.push(pos_upper.x, vva);
        }
        if(j.contains_vertex_at(pos_lower)){
            if(!j.is_main){
                VerticalVertexArray* vva = new VerticalVertexArray(pos_lower.x,v->bottom(), pos_lower.y);
                vva->set_color(sf::Color::Red);
                hva_lower.push(pos_lower.x, vva);
                j.at(pos_lower.x)->clear(); //TODO
            }
            else{
                v->pop(pos_lower.y - v->top()); //TODO
            }
        }
        if(j.contains_vertex_between(pos_lower, pos_upper) && !j.is_main){
            v->clear();
        }
    }

}

void NewTerrain::randomize(int roughness) {
    bool generating = true;
    int midpoint = const_screen_height / 2 + rnd::get_int(const_screen_height / -3, const_screen_height / 3);   //height-based midpoint off of which the next vertex' height should be decided
    if (roughness > midpoint){
        std::cout << "TANKS ERROR 0x00000000: Drawing limit exceeded. Try lowering roughness." << std::endl;
        return;
    }
    sf::Vector2i prev;
    sf::Vector2i next = sf::Vector2i(0, midpoint);

    HorizontalVertexArray hva;

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
            hva.push(x1 + j, new VerticalVertexArray(x1 + j, const_screen_height, y1));
        }
    }
    hva.is_main = true;
    main_terrain = hva;//.emplace_back(hva);
}

void NewTerrain::fall() {
    //NEW FALL VERTEX LOGIC
}

void NewTerrain::logic() {
    for(auto& hva : falling_terrains){
        if(hva.is_falling){
            hva.move(1, main_terrain);
        }
        else{
            //main_terrain.push(hva);
        }
    }
}

HorizontalVertexArray* NewTerrain::get_terrain() {
    return &main_terrain;
}

void NewTerrain::tidy() {
    int cnt = 0;
    for(auto& t: falling_terrains){
        t.tidy();
        if(t.count() == 0){
            cnt++;
        }
    }
    //std::cout << cnt << " empty falling_terrains" << std::endl;
}

std::vector<HorizontalVertexArray> NewTerrain::get_terrains() {
    std::vector<HorizontalVertexArray> vec = falling_terrains;
    vec.emplace_back(main_terrain);
    return vec;
}
