#include "Model_Leaderboard.h"
#include <fstream>
#include <algorithm>

//load save input logic of leaderboard
Leaderboard::Leaderboard(std::string file) : filename(file)
{
    load();
}

//load data
void Leaderboard::load()
{
    entries.clear();
    std::ifstream in(filename);
    std::string n;
    int s;
    while (in >> n >> s)
    {
        entries.push_back({n, s}); //store data
    }
}

//save data
void Leaderboard::save()
{
    std::ofstream out(filename);
    for (ScoreEntry &e : entries)
    {
        out << e.name << " " << e.score << "\n"; //write data
    }
}

//add score in leaderborad and set top 10
void Leaderboard::add_score(const std::string &name, int sc)
{
    entries.push_back({name, sc}); 

    std::sort(entries.begin(), entries.end(),
        [](const ScoreEntry &a, const ScoreEntry &b)
        {
            return a.score > b.score;
        });

    if (entries.size() > 10) entries.resize(10);
    save();
}

const std::vector<ScoreEntry>& Leaderboard::get_entries() const
{
    return entries;
}
