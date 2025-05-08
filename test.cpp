#include <iostream>
#include <vector>
#include <cstring>
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
    7. Dying mechanic: ensure that dead candidates/voters are sterilized(they do not posses the rigth to fullfil their functionality anymore)
*/


int main(){

    Election* e=new Election();
    if(!e->add_voivodship("Chicago", 3)) cerr<<"Voivodship Chicago not added, despite being unique"<<endl;
    if(!e->add_voivodship("Boston", 3)) cerr<<"Voivodship Boston not added, despite being unique"<<endl;
    if(!e->add_voivodship("New York", 4)) cerr<<"Voivodship New York not added, despite being unique"<<endl;
    if(e->add_voivodship("Chicago", 12)) cerr<<"Voivodship Chicago added, despite being duplicate"<<endl;
    if(e->add_voivodship("Chicago",2)) cerr<<"Voivodship Boston added, despite being duplicate"<<endl;
    e->display_voivodships(); //Expected Chicago, Boston, New York

    Candidate* c1=e->register_candidate("John", 36, "Chicago");//Scenario2: correct age
    if(!c1) cerr<<"Candidate John not registered, despite being unique"<<endl;
    Candidate* c2=e->register_candidate("Jam", 20, "Boston"); //Scenario2: underage
    if(c2) cerr<<"Candidate Jam registered, despite being underage"<<endl;
    Candidate* c3=e->register_candidate("John", 59, "New York");
    e->die_candidate(c1->get_id());
    if(e->get_number_of_citizens("Chicago")!=2) cerr<<"Voivodship Chicago citizens not decremented after candidate death"<<endl;
    Candidate* c4=e->register_candidate("Jimbo", 77, "Chicago");
    if(!c4) cerr<<"Candidate Jimbo not registered, despite being borderline"<<endl;
    Candidate* c5=e->register_candidate("Jimbo", 77, "Chicago");
    if(!c5) cerr<<"Candidate Jimbo with mathcing params not registered"<<endl;
    Candidate* c0=e->register_candidate("Jasmin", 43, "Chicago");
    if(c0) cerr<<"Candidate registered, over the limit"<<endl; //Scenario2: citizens limit reached
    e->display_registered_candidates(); //John, Jimbo, Jimbo

    Voter* v0=e->register_voter("Samuel", 54, "Chicago");//Scenario1: citizens limit reached
    Voter* v00=e->register_voter("Sema", 17, "New York"); //Scenario1: underage
    if(v00) cerr<<"Voter Sema registered, despite being underage"<<endl;
    if(v0) cerr<<"Voter Samuel registered, despite being over the limit"<<endl;
    Voter* v1=e->register_voter("Samuel", 49, "New York"); //Scenario1: correct age
    if(!v1) cerr<<"Voter Samuel not registered, despite being of correct age"<<endl;
    Voter* v2=e->register_voter("Samson", 18, "New York"); //Scenario1: correct age
    if(!v2) cerr<<"Voter Samson not registered, despite being of correct age"<<endl;
    Voter* v3=e->register_voter("Samson", 18, "New York"); //Scenario1: duplicate voter
    if(!v3) cerr<<"Voter Samson registered, despite being duplicate"<<endl;
    e->display_registered_voters("New York"); //Samuel, Samson, Samson 3 3
    e->die_voter(v3->get_id());
    e->display_registered_voters("New York"); //Samuel, Samson ids 5 and 6 Samson id 0 2 2
    
    Voter* v4=e->register_voter("Carl", 70, "Boston"); //Scenario1: correct age
    if(!v4) cerr<<"Voter Carl not registered, despite being of correct age"<<endl;
    Voter* v5=e->register_voter("Clark", 23, "Boston");
    if(!v5) cerr<<"Voter Clark not registered, despite being of correct age"<<endl;

    Voter* v6=e->register_voter("Carl", 70, "Boston");
    if(!v6) cerr<<"Duplicate voter not registered, borderline citizens limit"<<endl;
    v6->submit_vote(e,*c3);
    if(c3->ref_support()==1){
        e->display_voters(c3); //expected carl
        e->die_voter(v6->get_id());
        e->display_voters(c3); //expected empty list
        if(c3->ref_support()!=1) cerr<<"Vote revoked after voter death"<<endl;
    }else cerr<<"Valid vote of a duplicate not casted"<<endl;
    v0->submit_vote(e,*c2); //Scenario3: invalid voter-invalid candidate Scenario 6
    if(v0->get_vote()) cerr<<"Voter v0 voted, despite being invalid"<<endl;
    v00->submit_vote(e,*c3); //Scenario3: invalid voter-valid candidate Scenario 6
    if(v00->get_vote() || c3->ref_support()==2) cerr<<"Voter v00 voted, despite being invalid"<<endl;
    v1->submit_vote(e,*c1); //Scenario3: valid voter-invalid candidate Scenario 6
    if(v1->get_vote()) cerr<<"Voter v1 voted, despite voting on invalid candidate"<<endl;
    
    v1->submit_vote(e,*c3); //Scenario3: valid voter-valid candidate
    if(!v1->get_vote() || c3->ref_support()!=2) cerr<<"Voter v1 did not vote, despite voting on valid candidate"<<endl;
    e->display_voters(c3); //Samuel

    v2->submit_vote(e,*c4);
    v4->submit_vote(e,*c3);
    v5->submit_vote(e,*c3);
    c4->submit_vote();
    c5->submit_vote();
    e->display_registered_candidates();
    e->determine_winner(); //Expected John ID 3 57.14%, Jimbo ID 4 28.57%, Jimbo ID 5 14.28%   Scenario 5
    //Election attendance: 87.5%. There were ten citizens initially, all citizens
    //Were deemed voters at some point in time. 
    //Three died, but one of them(v6) did only after voting, so he was counted into the election attendance.
    //There were 8 voters of which 3 candidates and of the candidates has not voted(c3).
    //The attendance is 7/8*100%=87.5% Scenario 4

    e->support_by_age_group(); //Scenario 5
    //Jimbo id 5 100% elders
    //Jimbo id 4 50% elders 50% young adults
    //John id 3 25% young adults 25% middle aged 50% elders
    e->display_local("New York"); //50% John id 3 50% Jimbo id 4    //Scenario 5
    e->display_local("Boston"); //100% John id 3
    e->display_local("Chicago"); //50% Jimbo id 4 50% Jimbo id 5

    return 0;
}