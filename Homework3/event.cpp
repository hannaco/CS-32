class Event
{
public:
	Event(string name)
	{
		m_name = name;
	}

	string name() const
	{
		return m_name;
	}

	virtual string need() const = 0;

	virtual ~Event() {}

	virtual bool isSport() const
	{
		return true;
	}

private:
	string m_name;
};

class BasketballGame: public Event
{
public:
	BasketballGame(string name) : Event(name)
	{}

	virtual ~BasketballGame()
	{
		cout << "Destroying the " << name() << " basketball game" << endl;
	}

	virtual string need() const
	{
		return "hoops";
	}

};

class Concert: public Event
{
public:
	Concert(string name, string genre) : Event(name)
	{
		m_genre = genre;
	}

	virtual ~Concert()
	{
		cout << "Destroying the " << name() << " " << m_genre << " concert" << endl;
	}

	virtual string need() const
	{
		return "a stage";
	}

	virtual bool isSport() const
	{
		return false;
	}

private:
	string m_genre;
};

class HockeyGame: public Event
{
public:
	HockeyGame(string name) : Event(name)
	{}

	virtual ~HockeyGame()
	{
		cout << "Destroying the " << name() << " hockey game" << endl;
	}

	virtual string need() const
	{
		return "ice";
	}

};
