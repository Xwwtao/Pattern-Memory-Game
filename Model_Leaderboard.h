#pragma once
#include <vector>
#include <string>


//struct of score
struct ScoreEntry
{
    std::string name;
    int score;
};


//class leaderboard
class Leaderboard
{
private:
    std::vector<ScoreEntry> entries;
    std::string filename;

public:
    Leaderboard(std::string file = "data.txt");

    void load();
    void save();
    void add_score(const std::string &name, int sc);

    const std::vector<ScoreEntry>& get_entries() const;
};
