#include <iostream>
#include <vector>
#include <map>
#include "classes.h"
using namespace std;

/*
Assumptions and Restrictions:
R1. Voter must be at least 18 years of age to vote.
R2. The number of voters cannot outnumber the number of citizens of voivodship. 
R3. Each vote has the same weight and can be submitted only once. 
R4. Candidates must be at least 35 years of age to part-take.
R5. Candidates can also be voters, they can vote.
R6. Candidates cannot have the number of votes greater than the total amount of votes.
*/


int main(){
    Voter* voter1 = new Voter("Marvin Beak", 18, "Masovian");
    Voter* voter2 =new Voter("Agent 5.5",24,"Lesser Poland");
    Voter* voter4=new Voter("Cheese",16,"Lesser Poland");//Invalid args
    Voter* voter5=new Voter("Jim Pork",20,"Great Poland");
    Voter* voter6=new Voter("Bob Bacon",21,"test");
    Voivodship v1("Masovian",100);
    Voivodship v2("Lesser Poland",40);
    Voivodship v3("Great Poland",20);
    Voivodship v4("test",0);
    // if(v4.register_voter(voter6)) cerr<<"Number of citizens is smaller than amount of registered voters"<<endl;
    // if(!v1.register_voter(voter1)){
    //     cerr<<"Valid voter not registered correctly"<<endl;
    // }else{
    //     if(v1.register_voter(voter1))
    //         cerr<<"Voter registered twice to a single voivodship"<<endl;
    // }
    // if(v2.register_voter(voter1)){
    //     cerr<<"Voter assigned to a different voivodship"<<endl;
    // }
    // if(v2.register_voter(voter4)){
    //     cerr<<"Voter of age<18 registered successfully"<<endl;
    // }
    // v2.register_voter(voter2);
    // v3.register_voter(voter5);
    // if(v1.number_of_voters()!=1) cerr<<"Invalid number of voters in v1"<<endl;
    // if(v2.number_of_voters()!=1) cerr<<"Invalid number of voters in v2"<<endl;
    // if(v3.number_of_voters()!=1) cerr<<"Invalid number of voters in v3"<<endl;

    // v1.display_registered_voters(); //Expected information of voter1
    // v2.display_registered_voters(); //Expected information of voter2
    // v3.display_registered_voters(); //Expected information of voter5
    // vector<Voivodship> voivodships={v1,v2,v3};
    // Election e(voivodships);
    // Candidate* c1=new Candidate("Tim Cheese",36,"Masovian");
    // Candidate* c2=new Candidate("John Pork",34,"Lesser Poland");
    // Candidate* c3=new Candidate("Tiger Sam",40,"Great Poland");
    // if(!e.register_candidate(c1)){
    //     cerr<<"Valid candidate not registered successfully"<<endl;
    // }if(e.register_candidate(c2)){
    //     cerr<<"Candidate with invalid age registered successfully"<<endl;
    // }
    // e.register_candidate(c3);
    // e.display_registered_candidates(); //Expected information abot c1, c3
    // e.distribute_candidates_to_voivodships();
    // c1->submit_vote();
    // if(c1->ref_support()!=1) cerr<<"Candidates support not incremented after self vote"<<endl;
    // c1->submit_vote();
    // if(c1->ref_support()==2) cerr<<"Candidates vote status not changed after initial self vote"<<endl;
    
    // voter1->submit_vote(*c1);
    // if(c1->ref_support()!=2) cerr<<"Invalid support of candidate c1 after valid Voter vote cast"<<endl;
    // v1.display_local_support(); //Expected: Masovian: Tim Cheese: 100%
    // voter2->submit_vote(*c3);
    // v2.display_local_support(); //Expected: Lesser Poland: Tiger Sam: 100%
    // voter5->submit_vote(*c1);
    // v3.display_local_support();// Expected: Great Poland: Tim Cheese: 100%
    
    // e.determine_winner(); //Expected: Tim Cheese:66.6%, Tiger Sam: 33.3%
    // e.refAttendance();//  3/160 *100(This is in percantages,160 is sum of citizens, 3 is the counter) 
    // e.support_by_age_group(); //Expected: Young adults:100% Middle aged:0% Elders:0%


    return 0;
}