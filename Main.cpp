#include <Siv3D.hpp>

class Graph {
	static constexpr int dist = 256;
	static constexpr int r = 10;

	int n, m;
	std::vector<std::vector<int>> g;
	std::vector<Circle> poses; //<x, y>
	std::vector<std::pair<double, double>> moves;//<x, y>

	bool isgraping = false;
	int grapingnum;

public:
	void reposing() {
		g.resize(n);
		poses.resize(n);
		moves.resize(n,std::make_pair(0,0));

		for (auto& pos : poses) {
			pos.x = Random() * Scene::Width();
			pos.y = Random() * Scene::Height();
			pos.r = r;
		}
	}

	void init() {
		Console.open();

		//std::cin >> n >> m;
		n = 16;
		m = n - 1;

		reposing();

		for (int i = 0; i < m;i++) {
			int a, b;
			//std::cin >> a >> b;
			a = i;
			b = i + 1;
			g[a].push_back(b);
		}
	}

	void addDistance() {
		for (int i = 0; i < n; i++) {
			for (auto& j : g[i]) {
				int length = hypot(poses[i].x - poses[j].x, poses[i].y - poses[j].y);
				int add = std::max((length / dist) , 0);
				if (dist > length) {
					add = -add;
				}
				if (poses[i].x < poses[j].x) {
					moves[i].first+=add;
					moves[j].first-=add;
				}
				else {
					moves[i].first-=add;
					moves[j].first+=add;
				}
				if (poses[i].y < poses[j].y) {
					moves[i].second+=add;
					moves[j].second-=add;
				}
				else {
					moves[i].second-=add;
					moves[j].second+=add;
				}
			}
		}

		for (int i = 0; i < n; i++) {
			poses[i].x += moves[i].first;
			poses[i].y += moves[i].second;
		}
	}

	void draw() {
		if (isgraping) {
			poses[grapingnum].x = Cursor::Pos().x;
			poses[grapingnum].y = Cursor::Pos().y;
			if (MouseL.up()) {
				isgraping = false;
				moves[grapingnum] = std::make_pair(0, 0);
			}
		}

		for (int i = 0; i < n;i++) {
			if (poses[i].mouseOver()) {
				if (MouseL.down() and not isgraping) {
					isgraping = true;
					grapingnum = i;
					poses[i].x = Cursor::Pos().x;
					poses[i].y = Cursor::Pos().y;
					poses[i].draw(Palette::Green);
				} else {
					poses[i].draw(Palette::Red);
				}
			} else {
				poses[i].draw();
			}
		}

		for (int i = 0; i < n; i++) {
			for (auto& j : g[i]) {
				Line(poses[i].x, poses[i].y, poses[j].x, poses[j].y).draw(5, Color(255,255,255,100));
			}
		}
	}
};

void Main(){
	Window::SetStyle(WindowStyle::Sizable);

	Graph g;
	g.init();

	while (System::Update()){
		g.draw();
		g.addDistance();
		if (KeySpace.down()) {
			g.reposing();
		}
	}
}
