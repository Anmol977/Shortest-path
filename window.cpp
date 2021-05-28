#include "window.h"

window::window(){
	newindow.create(sf::VideoMode(600, 600),"Path-Finder",sf::Style::Close);
	this->rectangles.reserve(380);
	this->boxstate.reserve(380);
	for (int i = 0; i <20; ++i)
	{
		for (int j = 0; j <20; ++j)
		{
			rectangles.push_back(new sf::RectangleShape(sf::Vector2f(30,30)));
		}
	}
	start = sf::Vector2i(0, 0);
	end = sf::Vector2i(1, 1);
	initgrid(start,end);
}

void window::setend(sf::Vector2i newend) {
	end = newend;
}

void window::setstart(sf::Vector2i newstart) {
	start = newstart;
}

void window::initgrid(sf::Vector2i start, sf::Vector2i end) {
	for (int i = 0; i < 20; i++) {
		for(int j = 0; j < 20; j++) {
			if (i == start.x && j == start.y) {
				rectangles.at(i*20+j)->setFillColor(sf::Color(66,8,98));
				boxstate.push_back(1);
			}
			else if (i ==end.x && j ==end.y) {
				rectangles.at(i*20+j)->setFillColor(sf::Color(197,114,255));
				boxstate.push_back(2);
			}
			else {
				rectangles.at(i * 20 + j)->setFillColor(sf::Color::White);
				boxstate.push_back(0);
			}
			rectangles.at(i * 20 + j)->setPosition(sf::Vector2f(i * 30, j * 30));
			rectangles.at(i * 20 + j)->setOutlineColor(sf::Color::Black);
			rectangles.at(i * 20 + j)->setOutlineThickness(1);
		}
	}
}

void window::drawgrid(sf::RenderWindow& window) {
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			window.draw(sf::RectangleShape(*rectangles.at(i * 20 + j)));
		}
	}
}

void window::addPath() {
	int i, j, k = 0;
	cout << "path : " << endl;
	for (auto step : path)
	{
		if ((step.x != getStart().x || step.y != getStart().y) && (step.x != getEnd().x || step.y != getEnd().y))
		{
			i = step.x;
			j = step.y;
			cout<< i << " " << j << endl;
			boxstate.at(i * 20 + j) = 4;
			rectangles.at(i * 20 + j)->setFillColor(sf::Color::Yellow);
			k++;
		}
	}
	cout << "length of path: " << k << endl;
}

bool window::isvisitednode(int x, int y, std::vector<sf::Vector2i>& coords){
	for (int i = 0; i < coords.size(); ++i)
	{
		if (coords.at(i).x == x && coords.at(i).y == y)
			return true;
	}
	return false;
}

void window::checknode(int x, int y, int z, std::vector<sf::Vector2i>& visitednodes, std::vector<sf::Vector3i>& adjnodeslist) {
	if (x > 19 || y > 19 || y < 0 || x < 0)
	{
		return;
	}
	else if (!isWall(x, y) && !isvisitednode(x, y, visitednodes))
		{
			adjnodeslist.emplace_back(x, y, z);
			visitednodes.emplace_back(x, y);
		}
}

void window::findpath() {
	path.emplace_back(getStart().x, getStart().y, 0);
	vector<sf::Vector3i> adjnodeslist;
	vector<sf::Vector2i> visitednodes;
	adjnodeslist.emplace_back(path.at(0));
	visitednodes.emplace_back(path.at(0).x, path.at(0).y);
	int cont=500;
	while (cont--) {
		int noofnodes = adjnodeslist.size();
		for (int i = 0; i < noofnodes; i++) {
			auto step = adjnodeslist.at(i);
			checknode(step.x, step.y - 1, path.size() - noofnodes + 1 + i, visitednodes, adjnodeslist);
			checknode(step.x + 1, step.y, path.size() - noofnodes + 1 + i, visitednodes, adjnodeslist);
			checknode(step.x, step.y + 1, path.size() - noofnodes + 1 + i, visitednodes, adjnodeslist);
			checknode(step.x - 1, step.y, path.size() - noofnodes + 1 + i, visitednodes, adjnodeslist);
			checknode(step.x + 1, step.y - 1, path.size() - noofnodes + 1 + i, visitednodes, adjnodeslist);
			checknode(step.x + 1, step.y + 1, path.size() - noofnodes + 1 + i, visitednodes, adjnodeslist);
			checknode(step.x - 1, step.y + 1, path.size() - noofnodes + 1 + i, visitednodes, adjnodeslist);
			checknode(step.x - 1, step.y - 1, path.size() - noofnodes + 1 + i, visitednodes, adjnodeslist);
		}
		adjnodeslist.erase(adjnodeslist.begin(), adjnodeslist.begin() + noofnodes);
		for (auto step : adjnodeslist)
		{
			path.push_back(step);
			if(step.x!=getEnd().x||step.y!=getEnd().y)
				rectangles.at(step.x * 20 + step.y)->setFillColor(sf::Color::Cyan);
			if (step.x == getStart().x && step.y == getStart().y)
			{
				return;
			}
		}
	}	
}

void window::finalpath() {
	vector<sf::Vector3i> temp;
	sf::Vector3i end;
	for (auto step : path)
	{
		if (step.x == getEnd().x && step.y == getEnd().y)
		{
			end = step;
		}
	}
	temp.push_back(end);
	int i = end.z;
	while (i > 0)
	{
		temp.push_back(path.at(i - 1));
		i = path.at(i - 1).z;
	}
	path.erase(path.begin(), path.end());
	for (auto step : temp)
	{
		path.push_back(step);
	}
}

bool window::isWall(int x, int y) {
	if (x > 19 || y > 19 || y < 0 || x < 0)
		return 1;
	return boxstate.at(x * 20 + y) == 3;
}

sf::Vector2i window::convertmousepostoint(sf::Vector2i mousepos) {
	sf::Vector2i intpos;
	intpos.x= (mousepos.x - (mousepos.x % 30)) / 30;
	intpos.y= (mousepos.y - (mousepos.y % 30)) / 30;
	return intpos;
}

void window::removebox(sf::Vector2i mousepos) {
	sf::Vector2i pos;
	pos = convertmousepostoint(mousepos);
	if(pos.x>19 ||pos.y>19||pos.x<0||pos.y<0)
		return;
	else {
		boxstate.at(pos.x * 20 + pos.y) = 0;
		rectangles.at(pos.x * 20 + pos.y)->setFillColor(sf::Color::White);
	}
}

void window::addobstacles(sf::Vector2i mousepos) {	
	sf::Vector2i pos;
	pos = convertmousepostoint(mousepos);
	if (pos.x > 19 || pos.y > 19 || pos.x < 0 || pos.y < 0)
		return;
	else {
		boxstate.at(pos.x * 20 + pos.y) = 3;
		rectangles.at(pos.x * 20 + pos.y)->setFillColor(sf::Color(12,53,71));
	}
}

void window::setbeginbox(sf::Vector2i mousepos) {
	sf::Vector2i pos;
	pos = convertmousepostoint(mousepos);
	if (pos.x > 19 || pos.y > 19 || pos.x < 0 || pos.y < 0)
		return;
	else {
		setstart(pos);
		initgrid(pos, end);
	}
}

void window::setendbox(sf::Vector2i mousepos) {
	sf::Vector2i pos;
	pos = convertmousepostoint(mousepos);
	if (pos.x > 19 || pos.y > 19 || pos.x < 0 || pos.y < 0)
		return;
	else {
		setend(pos);
		initgrid(start, pos);
	}
}

sf::Vector2i window::getStart() 
{
	return start;
}

sf::Vector2i window::getEnd()
{
	return end;
}

void window::run() {
	sf::Clock clock;
	const sf::Time timePerFrame = sf::seconds(1.0f / 120.0f);
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (newindow.isOpen()) {
		processevents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processevents();
			render();
		}
	}
}

void window::processevents() {
	sf::Event event;
	while (newindow.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			newindow.close();
		if (event.type == sf::Event::KeyPressed)
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				setbeginbox(sf::Mouse::getPosition(newindow));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				setendbox(sf::Mouse::getPosition(newindow));
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				cout << "start: " << start.x<<" "<<start.y << endl;
				cout << "end: " << end.x << " " << end.y << endl;
				findpath();
				finalpath();
				addPath();
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			addobstacles(sf::Mouse::getPosition(newindow));
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			removebox(sf::Mouse::getPosition(newindow));
		}
	}
}

void window::render() {
	newindow.clear(sf::Color::White);
	drawgrid(newindow);
	newindow.display();
}