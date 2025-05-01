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
    if(v3==nullptr) cerr<<"Voter registred despite not citizens limit reached"<<endl;
    e->display_registered_voters("Chicago");//Expected output(names): John

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
    e->display_registered_candidates();//Expected output: empty list


    return 0;
}