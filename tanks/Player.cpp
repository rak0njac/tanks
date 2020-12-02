#include "Player.h"
#include <cmath>

const float pi = std::acos(-1);

sf::Vector2f closestPointOnLine(const sf::Vector2f& l1, const sf::Vector2f& l2, const sf::Vector2f& p) {
	float a1 = l2.y - l1.y;
	float b1 = l1.x - l2.x;
	double c1 = (l2.y - l1.y)*l1.x + (l1.x - l2.x)*l1.y;
	double c2 = -b1 * p.x + a1 * p.y;
	double det = (a1*a1 - (-b1 * b1));
	sf::Vector2f c;
	if (det != 0) {
		c.x =(float) ((a1*c1 - b1 * c2) / det); 
		c.y =(float) ((a1*c2 - (-b1*c1)) / det);
	}
	else {
		c.x = p.x;
		c.y = p.y;
	}
	return c;
}



Player::Player()
{
	body.setFillColor(sf::Color::Blue);
	body.setSize(sf::Vector2f(20,10));
	body.setOrigin(sf::Vector2f(10, 5));
	collisionCircle.setRadius(6);
	pos = sf::Vector2f(100, 0);
	collisionCircle.setFillColor(sf::Color::Red);
	body.setPosition(pos);
	collisionCircle.setOrigin(sf::Vector2f(6, 6));
	collisionCircle.setPosition(sf::Vector2f(pos.x,pos.y));
	movementVec.y = 0.f;
	movementVec.x = 0.f;

}


Player::~Player()
{
}

void Player::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(body, states);
	target.draw(collisionCircle, states);
}

void Player::move(const Terrain & terrain, sf::Vector2f mv)
{
#if 0
	int tocheck = static_cast<int>(roundf(pos.x));
	if (tocheck > 0 && tocheck < 800) {
		const Range* midtest = &terrain.ranges[tocheck];
		while (midtest->next) {
			midtest = midtest->next;
		}
	}
	//if (599-midtest->max < tocheck)
	//	return;
	movementVec *= 0.f;
	movementVec.y += fallSpeed;
	int direction = 0;
	if (movingRight) {
		movementVec.x += moveSpeed;
		direction = 1;
	}
	if (movingLeft) {
		movementVec.x -= moveSpeed;
		direction = -1;
	}
	for (int i = 0; i < 6; i++, tocheck+= direction) {
		//tocheck++;
		if (tocheck <= 0) continue;
		if (tocheck > 798) continue;
		const Range* current1 = &terrain.ranges[tocheck];
		const Range* current2 = &terrain.ranges[tocheck];
		if (movingRight) {
			current2 = &terrain.ranges[tocheck + 1];
		}
		else {
			current2 = &terrain.ranges[tocheck - 1];
		}
		while (current1->next) {
			current1 = current1->next;
		}
		while (current2->next) {
			current2 = current2->next;
		}
		
		sf::Vector2f p1(tocheck, 600 - (current1->max + 1));
		sf::Vector2f p2(tocheck + 1, 600 - (current2->max + 1));
		sf::Vector2f linePoint = closestPointOnLine(p1,p2, pos);

		if (linePoint.x > tocheck + 1) {
			linePoint.x = tocheck + 1;
		}
		if (linePoint.x < tocheck) {
			linePoint.x = tocheck;
		}
		float sqrdistance = squarePointDistance(linePoint, pos);
		sf::Vector2f cline(linePoint.x - pos.x, linePoint.y - pos.y);
		if (sqrdistance > ((collisionCircle.getRadius()/2 )*(collisionCircle.getRadius() / 2))) {
			//continue;
		}
		if (sqrdistance <= ((collisionCircle.getRadius() )*(collisionCircle.getRadius() ))) {
			//float angle =  std::abs(((std::atan((p2.y - p1.y) / (p2.x - p1.x))) * (180 / pi)))/90;
			//pos.x += (collisionCircle.getRadius() * (1-angle) -  cline.x);
			//pos.y += (collisionCircle.getRadius() * angle - cline.y);
			//collisionCircle.setPosition(pos);
			//body.setPosition(pos);
			float ration = std::abs(((std::atan((p2.y - p1.y) / (p2.x - p1.x))) * (180 / pi))/90);
			movementVec.x += movementVec.x * (1-ration);
			//movementVec.y = -movementVec.x * ration;
			if (movementVec.x > 0)
				movementVec.y += -movementVec.x * ration;
			else
				movementVec.y += movementVec.x * ration;
			//break;
		}
	}
	pos += movementVec;
	collisionCircle.setPosition(pos);
	body.setPosition(pos);
#endif

#if 1
	int circX = collisionCircle.getPosition().x;
	int circY = 600 - (collisionCircle.getPosition().y) - collisionCircle.getRadius();
	if (circX > 799) {
		circX = 799;
	}
	if (circX < 0) {
		circX = 0;
	}
	const Range* current = &terrain.ranges[circX];
	movementVec *= 0.f;
	int currentY;
	while (current) {
		if (circY > current->max && !current->next) {
			falling = true;
			movementVec.y += fallSpeed;
			break;
		}
		if (!current->next) {
			currentY = current->max;
		}
		current = current->next;
		
	}
	if (!current) {
		falling = false;
	}
	
	if (!falling) {
		if (movingRight) {
			int toMove = circX + static_cast<int>(moveSpeed);
			current = &terrain.ranges[toMove];
			while (current->next) {
				current = current->next;
			}

			if (toMove >= 795) {
				movementVec *= 0.f;
			}
			else if (current->max - currentY > 4) {
				movementVec *= 0.f;
			}
			else {
				float diff = (std::atan(current->max - currentY) * 180 / pi) / 90;
				movementVec.x += moveSpeed * (1 - diff);
				movementVec.y -= moveSpeed * diff;
				rotation = current->max - currentY;
			}
		}
		else if (movingLeft) {
			int toMove = circX - static_cast<int>(moveSpeed);

			current = &terrain.ranges[toMove];
			while (current->next) {
				current = current->next;
			}
			if (toMove <= 5)
				movementVec *= 0.f;
			else if (current->max - currentY > 4) {
				movementVec *= 0.f;
			}
			else {
				float diff = (std::atan(current->max - currentY) * 180 / pi) / 90;
				movementVec.x -= moveSpeed * (1 - diff);
				movementVec.y -= moveSpeed * diff;
				rotation = current->max - currentY;
			}
		}
	}
	pos += movementVec;
	collisionCircle.setPosition(pos);
	body.setPosition(pos);
	body.setRotation(rotation);
#endif
}
