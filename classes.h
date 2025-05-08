#ifndef CLASSES_H
#define CLASSES_H
#include <vector>
#include <iostream>
using namespace std;

class Candidate;
class Voter;
class Election;
class Voivodship;

//Purpose: Serve as an API, Coordinate between all inner instances, Validate
class Election{
    private:
        vector<Voivodship*> voivodships; //Vector containing pointers to Voivodship objects
        struct Candidates{  //Singly linked list of Candidate objects.
            Candidate* candidate; //Candidate object.
            Candidates* next; //Next Candidate instance.
        };
        Candidates* headC; //Head of the singly linked list for candidates.

        //Returns the election attendance in percantages as a double, used internally in determine_winner().
        double election_attendance();
        //Returns the candidate node based on submitted id, used for internal purposes.
        Candidate* get_candidate_node(unsigned int id);
        //Returns the voter node based on submitted id, used for internal purposes.
        Voter* get_voter_node(unsigned int id);
        //Returns the number of votes of candidates in a given voivodship.
        unsigned int get_votes_of_candidates(const char* voivodship_name);
        //Generates a unique ID that is not used by any other candidate or voter.
        unsigned int generate_id();
    public:
        Election();
        ~Election();
        //Checks if a candidate of given id is registered. Returns true if found and false otherwise.
        bool find_candidate(const unsigned int id);
        /*Adds a voivodship to the vector of voivodships. Returns true if the voivodship was added, false otherwise(there was such a voivoship already there).
        Takes a voivodship_name and number of citizens within it as arguments.*/
        bool add_voivodship(const char* voivodship_name, const unsigned int citizens); 
        //Determines an election winner and displays total support of each candidate in percantages. Additionally, displays the attendance.
        void determine_winner();
        //Shows the backing of candidates in percantages with respect to the age group of voters. Age(18-39]= Young adults, Age[40,65)= Middle aged, Age(65,..)= Elders. 
        void support_by_age_group();
        //Displays all registered candidates(without their support).
        void display_registered_candidates();
        //Displays all registered voters(they are stored across the voivodship objects).
        void display_all_voters();
        /*Registers a voter(creates instance) IF restrictions are met. Returns pointer to object if registered and nullptr otherwise.
        Takes voter's name, age and voivodship that he/she lives in(such a voivodship must be present in voivodship vector).*/
        Voter* register_voter(const char* name, const unsigned int age,const char* voivodship_name);
        /*Displays all registered voters(who voted) within a voivodship. 
        If such a voivodship does not exist false is returned and no voters displayed.
        Takes voivodship name as anh argument.*/
        bool display_registered_voters(const char* voivodship_name);
        /*Registers a candidate, creates such instance if the restrictions are met, appends the object to the singly linked list.
        Takes candidate's name, age and voivodship arguments required for such object creation.*/
        Candidate* register_candidate(const char* name, const unsigned int age, const char* voivodship_name);
        //Displays local support of Candidates in given voivodship. If voivodship of given name does not exist false is returned and true otherwise
        bool display_local(const char* voivodship_name);
        //Displays all voters that submitted their vote on the candidate.
        void display_voters(const Candidate* candidate);
        //Returns local support of a given candidate in a given voivodship.If either does not exist, false is returned.
        bool local_support(const Candidate* candidate,const char* voivodship_name);
        //Displays all voivodships(that are present in the vector of voivodship)
        void display_voivodships();
        /*Revokes voter's rigths to further participate in the election. The object becomes static, can't introduce any new
        data that would alter the program's numerical values. In practise the ID is set to 0, which is reserved for dead people.*/
        void die_voter(unsigned int voter_id);
        /* Marks the candidate as sterile, unable to further influence the program.
        In practise the ID is set to 0, which is reserved for dead people. */
        void die_candidate(unsigned int candidate_id);
        //Returns the number of citizens in a given voivodship based on provided voivodship name.
        unsigned int get_number_of_citizens(const char* voivodship_name);
};

//Purpose: participate in election by vote submission, member of inner data structures for other classes.
class Voter{
    private:
        unsigned int id; /*ID of a voter. Assigned at creation, unique for each alive voter. 
        The id 0 means that the object is dead(there may be multiple objects of ID 0), the ids>=1 are unique.*/
        char* name;//Name of a voter.
        unsigned int age;//Age of a voter.
        bool vote;//Boolean status of vote submission.
        char* voivodship;//Name of a voivodship the voter lives in.
    public:
        //Voter constructor,the vote submission status is assumed to be false at object creation.
        Voter(const unsigned int id,const char* name, const unsigned int age,const char* voivodship,bool vote=false);
        ~Voter();
        //Returns the voting status of a candidate
        bool get_vote()const;
        //Submits the vote for a candidate, increasing his backing and setting the vote submission status to true.
        void submit_vote(Election* e,Candidate& candidate);
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
        //Returns the ID of a voter
        unsigned int get_id()const;
        //Reference for id variable
        unsigned int& refId();
};


//Purpose: voote, get votes, store supporters(those who voted on the particular object).
//Candidate inherits from Voter class as candidates do have the voting rights, despite part-taking in an election.
class Candidate:public Voter{
    private:
        unsigned int support; //Numerical value representing the amount of votes  a candidate has gathered. At creation initialized to zero.
        struct Supporters{ //Singly linked list of voters who submitted the votes on the candidate.
            Voter* voter; //Voter instance.
            Supporters* next; //Next supporters instance.
        };
        Supporters* headS; //Head of the supporters structure.
        //Frees the list of supporters
        void free_supporters(); 
    public:
        //Constructor for candidate object, inherits from voter instance
        Candidate(const unsigned int id,const char* name, const unsigned int age, const char* voivodship, const bool vote = false, const int support = 0);
        ~Candidate();
        //Vote submission funcionality, increasing backing.
        void submit_vote();
        //Returns the candidate's support as a reference
        unsigned int& ref_support();
        //Appends a voter to the supporters list. Takes a voter node as an argument
        void add_supporter(Voter* voter);
        //Returns the number of supporters in a given voivodship.
        unsigned int supporters_in_voivodship(const char* voivodship);
        //Displays the candidate's supporters
        void display_supporters()const;
        /*Returns a vector of age distribution according to the formula
        Age(18-39]= Young adults, Age[40,65)= Middle aged, Age(65,..)= Elders. 
        The first index(zero index) is the number of young adults, 
        second index(index one) is the number of middle aged, third index(and last-index two) is the number of elders*/
        vector<int> age_distribution();
        //operator<< for candidate information
        friend ostream& operator<<(ostream& os,const Candidate& candidate); 
        //Deletes a supporter from the list of supporters. Given supporter(voter) id. 
        void delete_supporter(unsigned int id);
};

class Voivodship{
    private:
        char* name; //Name of the voivodship.
        unsigned int citizens; /*Number of all citizens with voting privellages
        (not all citizens are registered voters) living within the voidvodship. 
        The number voters can not exceed this number.*/
        struct Voters{ //Singly linked list of all registered voters.
            Voter* voter; //Voter instance.
            Voters* next; //Next Voters entry.
        };
        Voters* headV; //Head of singly linked list of all registered voters.

        //Clears the list of voters.
        void free_voters();
    public:
        Voivodship(const char* name, const unsigned int citizens);
        ~Voivodship();
        //Checks if a voter of given id is present(return true) or not(return false).
        bool find(const unsigned int id);
        //Returns the number of all registered voters.
        unsigned int number_of_voters();
        //Decreases the number of citizens by one. In the case of death/ Candidate registration(arithemtic which is later adjusted).
        void decrease_voter_count();
        //Increases the number of citizens(part of arithemtic related to candidate registration).
        void increase_voter_count();
        //Returns the number of citizens
        unsigned int number_of_citizens();
        //Returns the name of the voivodship
        char* get_name();
        //Displays all voters within voivodship that voted.
        void display_voters();
        //Adds a voter to the list of voters, based on provided voter node.
        bool add_voter(Voter* voter);
        //Returns the number of voters that submitted their vote, not neccessarily all registered voters submitted one.
        unsigned int number_of_submitted_votes();
        //Deletes a voter from the list of voters based on provided voter's id.
        bool delete_voter(unsigned int voter_id);
        //Returns the voter node based on submitted id.
        Voter* get_voter(unsigned int id); 
};

#endif