#include <vector>
#include <map>
using namespace std;


class Election{
    private:
        unsigned int counter; //Integer variable storing the amount of all votes.
        vector<Voivodship> voivodships; //Vector containing Voivodship objects. It is meant for determining the election winner as well the support in respect to group age.
        struct Candidates{  //Singly linked list of Candidate objects.
            Candidate* candidate; //Candidate object.
            Candidates* next; //Next Candidate instance.
        };
        Candidates* headC; //Head of the singly linked list for candidates.
    public:
        //Constructor for the election, the counter(amount of all votes) is assumed to be zero initially, takes the vector of voivodship objects as an argument as well.
        Election(const vector<Voivodship>& voivodship,const unsigned int counter=0); 
        //Destructor for Election class.
        ~Election();
        //Determines an election winner and displays total support of each candidate in percantages.
        void determine_winner();
        //Shows the backing of candidates with respect to the age group of voters. Age(18-39]= Young adults, Age[40,65)= Middle aged, Age(65,..)= Elders. 
        void support_by_age_group();
        //Appends the candidate to the structure of Candidates following prior validations. Takes Candidate node to be added as an argument.
        bool register_candidate(Candidate* candidate);
        //Displays all registered candidates(without their support).
        void display_registered_candidates();
        //Sends the list of candidates to all the voivodships.
        void distribute_candidates_to_voivodships();
        //Returns the counter private member as a reference.
        unsigned int& refAttendance();
};

class Voter{
    protected:
        bool validity;//boolean validity of a voter, indicating registartion status, by default false
        char* name;//Name of a voter.
        unsigned int age;//Age of a voter.
        bool vote;//Boolean status of vote submission.
        char* voivodship;//Name of a voivodship the voter lives in.
    public:
        //Constructor of voter, taking up the private variables. The vote submission status is assumed to be false at object creation.
        Voter(const char* name, const unsigned int age,const char* voivovship,bool vote=false,const bool validity=false);
        //Destructor for voter instance.
        ~Voter();
        //Submits the vote for a candidate, increasing his backing and setting the vote submission status to true.
        void submit_vote(Candidate& candidate);
        //Returns the age of a voter.
        unsigned int get_age();
        char* get_voivodship();
        //Returns the vote submission status.
        bool has_voted();
        //Displays the voters information.
        friend ostream& operator<<(ostream& os,const Voter& voter);
        //Returns the name of a voter
        char* get_name();
        //Returns the voivodship of voter
        char* get_voivodship();
};

//Candidate inherits from Voter class as candidates do have the voting rights, despite part-taking in an election.
class Candidate:public Voter{
    private:
        char* name; //Derived from Voter class
        unsigned int age; //Derived from Voter class
        bool validity; //Derived from Voter class
        char* voivodship; //Derived from Voter class
        bool vote; //Derived from Voter class
        unsigned int support; //Numerical value representing the amount of votes  a candidate has gathered. At creation initialized to zero.
        struct Supporters{ //Singly linked list of voters who submitted the votes on the candidate.
            Voter* voter; //Voter instance.
            Supporters* next; //Next supporters instance.
        };
        Supporters* headS; //Head of the supporters structure.
    public:
        //Constructor for candidate object, inherits from voter instance
        Candidate(const char* name,const unsigned int age, const char* voivodship,const bool vote=false,const bool validity=false,const int support=0): Voter(name, age, voivodship,vote,validity){}
        //Destructor for candidate instance.
        ~Candidate();
        //Vote submission, increasing backing
        void submit_vote();
        //Returns the candidate's support as a reference
        unsigned int& ref_support();
        //Displays all voters that submitted their vote on the candidate.
        void display_voters();
        //Appends a voter to the supporters list.
        void add_supporter(Voter* voter);
        //Frees the list of supporters
        void free_supporters(); 
        //operator<< for candidate information
        friend ostream& operator<<(ostream& os,const Candidate& candidate); 
};

class Voivodship{
    private:
        char* name; //Name of the voivodship.
        unsigned int citizens; //Number of all citizens living within the voidvodship. The number voters can not exceed this number.
        struct Voters{ //Singly linked list of all registered voters(who submitted their vote).
            Voter* voter; //Voter instance.
            Voters* next; //Next Voters entry.
        };
        Voters* headV; //Head of singly linked list of all registered voters.
        map<Candidate*, int> localVotes; //Map of Candidates and their backing WITHIN THE VOIVODSHIP.
    public:
        //Constructor for the voivodship instance.
        Voivodship(const char* name, const unsigned int citizens);
        //Destructor for voivodship object.
        ~Voivodship();
        //Appends the voter to the registerd voters.
        bool register_voter(Voter* voter);
        //Displays all registered voters within a voivodship.
        void display_registered_voters();
        //Iterates thorugh singly linked list and returns
        bool find(const char* name,const unsigned int age);
        //Displays the local support of each candidate in percantages.
        void display_local_support();
        //Returns the number of all registered voters.
        unsigned int& number_of_voters();
        //Returns the number of citizens
        unsigned int number_of_citizens();
        //Frees the singly linked list of voters
        void free_voters();
};