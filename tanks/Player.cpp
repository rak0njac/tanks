#include "Player.h"
#include <stdio.h>
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
	body.setOrigin(sf::Vector2f(10, 10));

	tube.setFillColor(sf::Color::Green);
	tube.setSize(sf::Vector2f(15, 3));
	tube.setOrigin(sf::Vector2f(0, 1.5));

	collisionCircle.setRadius(6);

	pos = sf::Vector2f(100, 0);

	collisionCircle.setFillColor(sf::Color::Red);

	body.setPosition(pos);
	tube.setPosition(pos);
	collisionCircle.setOrigin(sf::Vector2f(6, 6));
	collisionCircle.setPosition(sf::Vector2f(pos.x,pos.y));
	movementVec.y = 0.f;
	movementVec.x = 0.f;

}


Player::~Player()
{
}

void Player::shoot(Weapon& weapon){}

void Player::shoot(std::list <Projectile*>& projectiles) {
	if (weapons.size() <= 0) return;
	weapons[currentWeapon].fire(tube.getPosition() , tube.getRotation(), projectiles);
}

void Player::add_wep(Weapon wep)
{
	weapons.push_back(wep);
}

void Player::change_wep(int i)
{
	currentWeapon += i;
	if (currentWeapon < 0) currentWeapon = weapons.size() - 1;
	else currentWeapon %= weapons.size();
}

void Player::change_wep_abs(int i)
{
	currentWeapon = i % weapons.size();
}

void Player::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(tube, states);
	target.draw(body, states);
	target.draw(collisionCircle, states);
	
}

void Player::update(
	const Terrain &terrain,
	sf::Vector2f mv,
	const sf::Vector2f & mousepos,
	GameResourceManager& grm)
{
	if (firing) shoot(grm.projectiles);

	move(terrain, mv, mousepos);
}

void Player::move(const Terrain & terrain, sf::Vector2f mv, const sf::Vector2f &mousepos)
{
	float tube_angle = atan2(mousepos.y - pos.y, mousepos.x - pos.x);
	tube_angle *= 180 / pi;
	tube.setRotation(tube_angle);
	//tube.rotate(tube_angle);
	

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
	//int circX = collisionCircle.getPosition().x;
	//int circY = 600 - (collisionCircle.getPosition().y) - collisionCircle.getRadius();
	int circX = body.getPosition().x;
	int circY = 600 - body.getPosition().y;// -body.getPosition().x;
	if (circX > 799) {
		circX = 799;
	}
	if (circX < 0) {
		circX = 0;
	}
	const Range* current = &terrain.ranges[circX];
	movementVec *= 0.f;
	float ang = rotation;
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
	

	// HERE START CALCULATING NORMAL BETWEEN CURRENT AND FIRST CLOSEST VERTEX AND SETTING THE ROTATION BASED ON THAT RESULT
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
				ang = -60*diff;
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
				ang = 45 * diff;
			}
		}
	}
	rotation = ang;
	pos += movementVec;
	collisionCircle.setPosition(pos);
	body.setPosition(pos);
	body.setRotation(ang);
	tube.setPosition(pos);
	//body.setRotation(rotation);
#endif
#if 0
	float circX = collisionCircle.getPosition().x;
	float circY = 600 - (collisionCircle.getPosition().y) - collisionCircle.getRadius();
	int hX = ceil(circX);
	int lX = floor(circX);
	int hY = ceil(circY);
	int lY = floor(circY);
	if (circX > 799) {
		circX = 799;
	}
	if (circX < 0) {
		circX = 0;
	}
	const Range* currentLower = &terrain.ranges[lX];
	const Range* currentHigher = &terrain.ranges[hX];
	movementVec *= 0.f;
	while (currentLower->next) currentLower = currentLower->next;
	while (currentHigher->next) currentHigher = currentHigher->next;
	float midpos = (currentLower->max + currentHigher->max) / 2.f;
	if (circY > midpos) {
		falling = true;
		movementVec.y += fallSpeed;
	}
	else {
		falling = false;
	}

	float diff =atan(fmod(midpos,1.01)) *180/pi/70;
	float toMove = midpos + moveSpeed;
	int toMoveHigher = ceil(toMove);
	int toMoveLower = floor(toMove);
	//const Range* current;
	if (!falling) {
		if (movingRight) {
			if (toMove >= 795) {
				movementVec *= 0.f;
			}
			else if (currentHigher->max - midpos > 4) {
				movementVec *= 0.f;
			}
			else {
				movementVec.x += moveSpeed * (1-diff);
				movementVec.y -= moveSpeed * diff;
				//rotation = current->max - currentY;
			}
		}
		else if (movingLeft) {
			if (toMove <= 5)
				movementVec *= 0.f;
			else if (currentLower->max - midpos > 4) {
				movementVec *= 0.f;
			}
			else {
				movementVec.x -= moveSpeed * (1-diff);
				movementVec.y -= moveSpeed * diff;
				//rotation = current->max - currentY;
			}
		}
		//collision handling
		int steps = floor(moveSpeed);
		int nX = hX + 1;
		const Range* nextHigher = &terrain.ranges[nX];
		const Range* nextLower = currentHigher;
		/*while (steps > 0) {
			float diffNext = atan(fmod((nextHigher->max+nextLower->max)/2, 1.01)) * 180 / pi / 90;;
			if (diffNext > diff) {
				diff = diffNext;
			}
			if (movingLeft)
			{
				movementVec.x -=  (1 - diff);
				movementVec.y -=  diff;
			}
			else if(movingRight)
			{
				movementVec.x +=(1 - diff);
				movementVec.y -=  diff;
			}
			currentHigher = nextHigher;
			nextLower = currentHigher;
			currentLower = nextLower;
			nX++;
			steps--;
			nextHigher = &terrain.ranges[nX];

		}*/
		
	}

	pos += movementVec;
	collisionCircle.setPosition(pos);
	body.setPosition(pos);
	//body.setRotation(rotation);
#endif
#if 0
#endif
}


