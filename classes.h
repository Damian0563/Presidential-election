#ifndef CLASSES_H
#define CLASSES_H
#include <vector>
#include <iostream>
#include <map>
using namespace std;

class Candidate;
class Voter;
class Election;
class Voivodship;

class Election{
    private:
        vector<Voivodship*> voivodships; //Vector containing Voivodship objects
        struct Candidates{  //Singly linked list of Candidate objects.
            Candidate* candidate; //Candidate object.
            Candidates* next; //Next Candidate instance.
        };
        Candidates* headC; //Head of the singly linked list for candidates.
    public:
        //Constructor for the election
        Election();
        //Adds a voivodship to the vector of voivodships.Returns true if the voivodship was added, false otherwise(there was such a voivoship already there).
        bool add_voivodship(const char* name, const unsigned int citizens); 
        //Destructor for Election class.
        ~Election();
        //Determines an election winner and displays total support of each candidate in percantages.
        void determine_winner();
        //Shows the backing of candidates with respect to the age group of voters. Age(18-39]= Young adults, Age[40,65)= Middle aged, Age(65,..)= Elders. 
        void support_by_age_group();
        //Displays all registered candidates(without their support).
        void display_registered_candidates();
        //Displays all voters
        void display_all_voters();
        //Registers a voter(creates instance) IF restrictions are met. Returns pointer to object if registered and nullptr otherwise.
        Voter* register_voter(const char* name, const unsigned int age,const char* voivodship);
        //Displays all registered voters(who voted) within a voivodship. If such a voivodship does not exist false is returned and no voters displayed.
        bool display_registered_voters(const char* voivodship_name);
        //Registers a candidate, creates such instance if the restrictions are met, appends the object to the singly linked list.
        Candidate* register_candidate(const char* name, const unsigned int age, const char* voivodship);
        //Displays local support of Candidates in given voivodship
        bool display_local(const char* voivodship_name);
        //Returns the election attendance in percantage
        double election_attendance();
        //Displays all voters that submitted their vote on the candidate.
        void display_voters(const Candidate* candidate);
        //Returns local support of a given candidate in a given voivodship.If either does not exist, false is returned.
        bool local_support(const Candidate* candidate,const char* voivodship_name);
        //Displays all voivodships
        void display_voivodships();
        //Returns the number of votes of candidates in a given voivodship.
        unsigned int get_votes_of_candidates(const char* voivodship);
};

class Voter{
    private:
        char* name;//Name of a voter.
        unsigned int age;//Age of a voter.
        bool vote;//Boolean status of vote submission.
        char* voivodship;//Name of a voivodship the voter lives in.
    public:
        //Constructor of voter, taking up the private variables. The vote submission status is assumed to be false at object creation.
        Voter(const char* name, const unsigned int age,const char* voivodship,bool vote=false);
        //Destructor for voter instance.
        ~Voter();
        //Returns the voting status of a candidate
        bool get_vote()const;
        //Submits the vote for a candidate, increasing his backing and setting the vote submission status to true.
        void submit_vote(Candidate& candidate);
        //Returns the age of a voter.
        unsigned int get_age()const;
        //Displays the voting status and modifies it when vote is casted.
        bool& has_voted();
        //Displays the voters information.
        friend ostream& operator<<(ostream& os,const Voter& voter);
        //Returns the name of a voter
        char* get_name()const;
        //Returns the voivodship of voter
        char* get_voivodship()const;

};

//Candidate inherits from Voter class as candidates do have the voting rights, despite part-taking in an election.
class Candidate:public Voter{
    private:
        unsigned int support; //Numerical value representing the amount of votes  a candidate has gathered. At creation initialized to zero.
        struct Supporters{ //Singly linked list of voters who submitted the votes on the candidate.
            Voter* voter; //Voter instance.
            Supporters* next; //Next supporters instance.
        };
        Supporters* headS; //Head of the supporters structure.
    public:
        //Constructor for candidate object, inherits from voter instance
        Candidate(const char* name, const unsigned int age, const char* voivodship, const bool vote = false, const int support = 0);
        //Destructor for candidate instance.
        ~Candidate();
        //Vote submission, increasing backing
        void submit_vote();
        //Returns the candidate's support as a reference
        unsigned int& ref_support();
        //Appends a voter to the supporters list.
        void add_supporter(Voter* voter);
        //Frees the list of supporters
        void free_supporters(); 
        //Returns the number of supporters in a given voivodship.
        unsigned int supporters_in_voivodship(const char* voivodship);
        //Displays the candidate's supporters
        void display_supporters()const;
        //Function returning support with respect to age group
        vector<int> age_distribution();
        //operator<< for candidate information
        friend ostream& operator<<(ostream& os,const Candidate& candidate); 
};

class Voivodship{
    private:
        char* name; //Name of the voivodship.
        unsigned int citizens; //Number of all citizens living within the voidvodship. The number voters can not exceed this number.
        struct Voters{ //Singly linked list of all registered voters.
            Voter* voter; //Voter instance.
            Voters* next; //Next Voters entry.
        };
        Voters* headV; //Head of singly linked list of all registered voters.
    public:
        //Constructor for the voivodship instance.
        Voivodship(const char* name, const unsigned int citizens);
        //Destructor for voivodship object.
        ~Voivodship();
        //Iterates thorugh singly linked list and returns boolean value indicating presence of a voter in those who voted.
        bool find(const char* name,const unsigned int age);
        //Returns the number of all registered voters.
        unsigned int number_of_voters();
        //Increases the number of voters by one(decreases the citizens count). This is done in the case of candidate registration(the object can not be linked to the list).
        void add_voter_count();
        //Returns the number of citizens
        unsigned int number_of_citizens();
        //Returns the name of the voivodship(private memeber)
        char* get_name();
        //Clears the list of voters.
        void free_voters();
        //Displays all voters within voivodship that voted.
        void display_voters();
        //Adds a voter to the list of voters.
        bool add_voter(Voter* voter);
        //Returns the number of voters that submitted their vote, not neccessarily all registered voters submitted one.
        unsigned int number_of_submitted_votes();
};

#endif