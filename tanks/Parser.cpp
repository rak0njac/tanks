#include "Parser.h"
#include "Weapon.h"
#include <stdio.h>

std::vector<Weapon> Parser::parse(std::string file)
{
	//printf("1");
	std::vector<Weapon> weapons;
	std::string buf;
	char cget = 0;
	std::ifstream stream(file, std::ifstream::in);
	if (stream.fail()) {
		printf("Failed to open file!!!\n");
	}
	while ((cget = stream.get()) != EOF) {
		std::string name = " ";
		float speed = 3;
		float expl_size = 3;
		float proj_rad = 3;
		float cooldown = 0.3;
		float drop_rate = 3;
		int readchars = 0;
		WeaponType type = WeaponType::STANDARD;
		//printf("1\n");

		while (cget <= 40) stream.get(cget);
		while (( cget != '{' && cget != ':' && cget != '}')) {
			//printf("%c", cget);
			buf.push_back(cget);
			readchars++;
			cget = stream.get();
		}
		//printf("%d\n", readchars);
		//printf("2\n");
		readchars = 0;
		name = buf;
		buf.clear();
		//printf("3\n");
		if (cget == ':') {
			//printf(":\n");
			while (stream.peek() <= 40) stream.get();
			cget = stream.get();
			while (cget != '{') {

				if (cget >= 141 && cget <= 172)
				{
					cget -= 'A' - 'a';
				}
				buf.push_back(cget);
				readchars++;
				while (stream.peek() <= 40) stream.get();
				cget = stream.get();
			}
			readchars = 0;
			//printf("%s\n", buf.c_str());
			if (buf == "STANDARD") {
				type = WeaponType::STANDARD;
			}
			else if (buf == "LAZOR") {
				type = WeaponType::LAZOR;
			}
		}
		//printf("4\n");
		//printf("5\n");
		while (cget != '}') {
			float value = 0;
			buf.clear();
			cget = stream.get();
			while (cget <= 40) cget = stream.get();
			while ((cget != ':' && cget != '}') && cget>40) {
				
				buf.push_back(cget);
				readchars++;
				cget = stream.get();
			}
			while (cget <= 40) stream.get(cget);
			if (cget != ':') continue;
			stream >> value;
			if (buf == "speed") {
				speed = value;
			}
			else if (buf == "expl_size") {
				expl_size = value;
			}
			else if (buf == "proj_rad") {
				proj_rad = value;
			}
			else if (buf == "cooldown") {
				cooldown = value;
			}
		}
		//printf("%s %f %f %f %f %f\n",name.c_str(),expl_size,cooldown,speed,proj_rad,drop_rate);
		Weapon w(name, type, expl_size, cooldown, speed, proj_rad, drop_rate);
		weapons.push_back(w);

	}

	stream.close();
	return weapons;
}
