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
R7. The uniquness of objects is assumed, namely there can not be multiple voivodships with the same name.
R8. The same applies to voters, there can not be multiple voters with the same name and age IN A GIVEN VOIVODSHIP.
*/

/*
    IMPORTANT SCEANRIOS TO CHECK:
    1. Adding a voivodship with the same name twice.
    2. Registering a voter with the same name and age in a given voivodship twice.
    3. Registering a candidate with the same name and age in a given voivodship twice.
    4. Registering a candidate with the same name and age in a different voivodship.    


*/


int main(){

    Election* e=new Election();
    if(!e->add_voivodship("Chicago", 2)) cerr<<"Voivodship Chicago not added, despite being unique"<<endl;
    if(!e->add_voivodship("Boston", 3)) cerr<<"Voivodship Boston not added, despite being unique"<<endl;
    if(!e->add_voivodship("New York", 4)) cerr<<"Voivodship New York not added, despite being unique"<<endl;
    if(e->add_voivodship("Chicago", 12)) cerr<<"Voivodship Chicago added, despite being duplicate"<<endl;
    if(e->add_voivodship("Chicago",2)) cerr<<"Voivodship Boston added, despite being duplicate"<<endl;

    Voter* v1=e->register_voter("John", 18, "Chicago");
    if(v1==nullptr) cerr<<"Correct voter not registered, despite being 18"<<endl;
    Voter* v2=e->register_voter("Jack", 17, "Chicago");
    if(v2!=nullptr) cerr<<"Incorrect voter registered, despite being under age"<<endl;
    e->display_voivodships();//Expected output: Chicago, Boston, New York NO COPY!!!!!
    Voter* v3=e->register_voter("Jill", 19, "Chicago");
    if(v3==nullptr) cerr<<"Voter not registred despite not citizens limit reached"<<endl;
    e->display_registered_voters("Chicago");//Expected output(names): John Jill Number of voters: 2, Number of citizens: 2

    Voter* v4=e->register_voter("Jim",56,"New York");
    if(v4==nullptr) cerr<<"Correct voter not registered, despite being 56"<<endl;
    Voter* v5=e->register_voter("Jack",20,"Boston");
    if(v5==nullptr) cerr<<"Correct voter not registered, despite being 20"<<endl;
    Voter* v6=e->register_voter("Jack",20,"Boston");
    if(v6!=nullptr) cerr<<"Incorrect voter registered, despite being duplicate"<<endl;
    Voter* v7=e->register_voter("Sam",43,"Boston");
    if(v7==nullptr) cerr<<"Correct voter not registered, despite being 43"<<endl;
    Voter* v8=e->register_voter("Sam",43,"New York");
    if(v8==nullptr) cerr<<"Correct voter not registered, despite being duplicate BUT IN A DIFFERENT VOIVODSHIP"<<endl;
    Voter* v9=e->register_voter("Samuel",26,"Boston");
    if(v9==nullptr) cerr<<"Correct voter not registered, despite being 26 and borderline citizens case"<<endl;

    Candidate* c1=e->register_candidate("Pearl",65,"Chicago");
    if(c1!=nullptr) cerr<<"Candidate registered, violating maximum citizens limit"<<endl;
    e->display_registered_candidates();//Expected output: empty list(No candidates registered)
    Candidate* c0=e->register_candidate("Pam",30,"New York");
    if(c0!=nullptr) cerr<<"Candidate registered, despite being under presidential age"<<endl;
    Candidate* c2=e->register_candidate("Pearl",35,"New York");
    if(c2==nullptr) cerr<<"Candidate not registered, despite being 35 and no citizens limit reached"<<endl;
    Candidate* c3=e->register_candidate("Carl",35,"New York");
    if(c3==nullptr) cerr<<"Candidate not registered, despite being 35 and no citizens limit reached"<<endl;
    v1->submit_vote(*c2);
    if(!v1->has_voted() || c2->ref_support()!=1) cerr<<"Vote not submitted, despite being correct"<<endl;
    v1->submit_vote(*c3);
    if(c3->ref_support()==1) cerr<<"Vote submission allowed twice"<<endl;
    v2->submit_vote(*c2); //Invalid voter, should not be able to submit a vote(NULLPTR case handled- no seg fault)
    if(c2->ref_support()==2) cerr<<"Invalid voter submitted a vote"<<endl;
    v4->submit_vote(*c3);
    if(!v4->has_voted() || c3->ref_support()!=1) cerr<<"Valid voter did not modify its voting status. The support was not incremented"<<endl;
    v5->submit_vote(*c2);
    v7->submit_vote(*c2);
    v8->submit_vote(*c3);
    v9->submit_vote(*c2);
    c1->submit_vote(); 
    if(c1->ref_support()!=0) cerr<<"Invalid candidate submitted a vote"<<endl; //Invalid candidate, should not be able to submit a vote(NULLPTR case handled- no seg fault)
    c2->submit_vote();
    c3->submit_vote();
    e->display_all_voters();//Expected output: John, Jill,Samuel, Sam, Jack, Sam, Jim
    e->display_registered_candidates();//Expected output: Carl-3 votes, Pearl-5 votes
    e->determine_winner(); //Pearl: 62.5%, Carl: 37.5% Explanation, only v3 8/9*100%=88.89%. All votes are counted, also the candidate's.

    e->display_voters(c2);//Expected output: John, Jack, Sam, Samuel
    e->display_voters(c3);//Expected output: Sam, Jim
    if(!e->display_local("Chicago")) cerr<<"Voivodship Chicago not found"<<endl; //expected output: Chicago: Pearl: 100%, Carl:0%
    if(!e->display_local("Boston")) cerr<<"Voivodship Boston not found"<<endl; //expected output: Boston: Pearl: 100%, Carl: 0%
    if(!e->display_local("New York")) cerr<<"Voivodship New York not found"<<endl; //expected output: New York: Pearl: 25%, Carl: 75%
    if(e->display_local("Los Angeles")) cerr<<"Voivodship Los Angeles found, despite not being registered"<<endl;

    e->local_support(c2,"New York");//Expected output: Pearl: 25%
    e->local_support(c2,"Boston");//Expected output: Pearl: 100%
    e->local_support(c2,"Chicago");//Expected output: Pearl: 100%

    e->support_by_age_group();
    //Expected output: Pearl Young Adults:80%, Middle Aged: 20%, Elders: 0%
    //Expected output: Carl Young Adults: 33.33%, Middle Aged: 66.67%, Elders: 0%


    return 0;
}