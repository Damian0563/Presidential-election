#include <iostream>
#include <vector>
#include "classes.h"
using namespace std;

/*
Assumptions and Restrictions:
A1.Candidates and voters are unique by an arbitrary id.
A2.Voivodships are differentiated by name only.
    There can be only one voivodship Chicago in the election.
R1. Voter must be at least 18 years of age to vote.
R2. The number of voters cannot outnumber the number of citizens of voivodship. 
R3. Each vote has the same weight and can be submitted only once. 
R4. Candidates must be at least 35 years of age to part-take.
R5. Candidates can also be voters, they can vote.
R6. Candidates cannot have the number of votes greater than the total amount of votes.
R7. Candidates and Voters can die, if a vote was casted before death, the vote is still counted.
*/

/*
    IMPORTANT SCENARIOS TO CHECK:
    1. Voter registration: underage, correct age, citizens limit reached.
    2. Candidate registration: underage, correct age, citizens limit reached.   
    3. Vote submission: valid voter-valid candidate, valid voter-invalid candidate, invalid voter-valid candidate, invalid voter-invalid candidate.
    4. Election attendance: ensure that candidates are taken into consideration.
    5. Determining support(global,local,...etc.): ensure candidates' votes are counted alongside 'regular' voters.
    6. Handling of empty lists: ensure that no seg faults occur when trying to access INVALID NODES.
*/


int main(){

    Election* e=new Election();
    if(!e->add_voivodship("Chicago", 3)) cerr<<"Voivodship Chicago not added, despite being unique"<<endl;
    if(!e->add_voivodship("Boston", 3)) cerr<<"Voivodship Boston not added, despite being unique"<<endl;
    if(!e->add_voivodship("New York", 4)) cerr<<"Voivodship New York not added, despite being unique"<<endl;
    if(e->add_voivodship("Chicago", 12)) cerr<<"Voivodship Chicago added, despite being duplicate"<<endl;
    if(e->add_voivodship("Chicago",2)) cerr<<"Voivodship Boston added, despite being duplicate"<<endl;


    Candidate* c1=e->register_candidate("John", 36, "Chicago");//Scenario2: correct age
    if(!c1) cerr<<"Candidate John not registered, despite being unique"<<endl;
    Candidate* c2=e->register_candidate("Jam", 20, "Boston"); //Scenario2: underage
    if(c2) cerr<<"Candidate Jam registered, despite being underage"<<endl;
    Candidate* c3=e->register_candidate("John", 59, "New York");
    e->die_candidate(1);
    if(e->get_number_of_citizens("Chicago")!=2) cerr<<"Voivodship Chicago citizens not decremented after candidate death"<<endl;
    Candidate* c4=e->register_candidate("Jimbo", 77, "Chicago");
    if(!c4) cerr<<"Candidate Jimbo not registered, despite being borderline"<<endl;
    Candidate* c5=e->register_candidate("Jimbo", 77, "Chicago");
    if(!c5) cerr<<"Candidate Jimbo with mathcing params not registered"<<endl;
    Candidate* c0=e->register_candidate("Jasmin", 43, "Chicago");
    if(c0) cerr<<"Candidate registered, over the limit"<<endl; //Scenario2: citizens limit reached
    e->display_registered_candidates(); //John, Jimbo, Jimbo

    return 0;
}