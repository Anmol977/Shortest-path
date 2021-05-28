#pragma once
#include<iostream>
#include<SFML/Graphics.hpp>
#include<vector>

using namespace std;

class window
{
	sf::RenderWindow newindow;
	vector<sf::RectangleShape*> rectangles;
	vector<int> boxstate;
	sf::Vector2i start;
	sf::Vector2i end;
	vector<sf::Vector3i> path;

public:
	window();
	void setend(sf::Vector2i newend);
	void setstart(sf::Vector2i newend);
	void run();
	void processevents();
	void render();
	void findpath();
	void initgrid(sf::Vector2i start, sf::Vector2i end);
	void removebox(sf::Vector2i mousepos);
	void setbeginbox(sf::Vector2i mousepos);
	void setendbox(sf::Vector2i mousepos);
	void drawgrid(sf::RenderWindow& window);
	void addobstacles(sf::Vector2i mousepos);
	void addPath();
	void checknode(int x, int y, int z, std::vector<sf::Vector2i>& visitednodes, std::vector<sf::Vector3i>& adjnodelist);
	void finalpath();
	bool isvisitednode(int x, int y, std::vector<sf::Vector2i>& coords);
	bool isWall(int x, int y);
	sf::Vector2i getStart();
	sf::Vector2i getEnd();
	sf::Vector2i convertmousepostoint(sf::Vector2i pos);
};

