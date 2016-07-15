struct Node {
	const int number;

	explicit Node(int n);
};

struct Edge {
	const int v1, v2;  		// Номера соединённых вершин
	const weight_t weight;	// Вес (длина)
	const double eta;  		// Видимость
	const double eta_beta;  // Значение eta^beta
	double phe;  			// Количество феромона
	double phe_acc;  		// Количество феромона, оставленного муравьями
	double prob;  			// Вероятность выбора этого ребра

	Edge( int, int, double );  	// Конструктор, принимающий номера
								// соединяемых городов и
								// расстояние между ними (вес ребра)

	void update();  // Метод, обновляющий количество феромона и вероятность
};


struct CompleteGraph {
	VN nodes;  				// Список городов
	VE edges;  				// Список рёбер
	const size_t n;  		// Количество городов
	vector<VE> edges_map;  	// Матрица рёбер

	explicit CompleteGraph(const vector<vector<weight_t>>&);

	Node* get_node(int) const;
	Edge* get_edge( int, int ) const;
	Edge* get_edge( Node*, Node* ) const;
	void update();
};


struct Ant {
	CompleteGraph* const g;  	// Указатель на граф
	Node* const start_town;  	// Начальный город
	VN path;  					// Путь, пройденный муравьём
	weight_t path_len;  		// Длина этого пути
	valarray<bool> J;

	Ant( CompleteGraph*, size_t );

	void go();
	void compare_opt(VN&);
};
