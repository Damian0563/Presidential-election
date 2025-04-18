#include "classes.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

Election::Election(const vector<Voivodship*> voivodship,const unsigned int counter){
    this->voivodships=voivodship;
    this->counter=counter;
    this->headC=nullptr;
}

Election::~Election(){
    this->headC=nullptr;
}

Candidate::~Candidate(){
    this->free_supporters();
}

void Election::determine_winner(){
    if(headC!=nullptr){
        Candidates* temp=this->headC;
        int max=0,curr_support=0,total=0;
        Candidate* winner=this->headC->candidate;
        while(temp){
            curr_support=temp->candidate->ref_support();
            total+=curr_support;
            if(max<curr_support){
                max=curr_support;
                winner=temp->candidate;
            }
            temp=temp->next;
        }
        temp=this->headC;
        double percantage;
        cout<<"Winner: "<<winner->get_name()<<" gets "<<(max*100)/total<<"%"<<" support in general election."<<endl;
        while(temp){
            if(strcmp(winner->get_name(),temp->candidate->get_name())!=0){
                percantage=((temp->candidate->ref_support())*100)/total;
                cout<<temp->candidate->get_name()<<", support: "<<percantage<<"%"<<endl;
            }
            temp=temp->next;
        }
    }else cout<<"No candidates registered"<<endl;
}

void Election::support_by_age_group(){
    Candidates* temp;
    for(auto& voivodship:this->voivodships){
        double young_adults=0,middle_aged=0,elders=100;
        temp=this->headC;
        int total=temp->candidate->ref_support();
        while(temp){
            young_adults=(temp->candidate->age_distribution()[0]*100)/total;
            middle_aged=(temp->candidate->age_distribution()[1]*100)/total;
            elders=(temp->candidate->age_distribution()[2]*100)/total;
            cout<<temp->candidate->get_name()<<endl;
            cout<<"Young adults: "<<young_adults<<"%"<<endl;
            cout<<"Middle aged: "<<middle_aged<<"%"<<endl;
            cout<<"Elders: "<<elders<<"%"<<endl;
            temp=temp->next;
        }
    }
}

vector<int> Candidate::age_distribution(){
    int young_adults=0,middle_aged=0,elders=0;
    Supporters* temp=this->headS;
    while(temp){
        if(temp->voter->get_age()>=18 && temp->voter->get_age()<40) young_adults++;
        else if(temp->voter->get_age()>=40 && temp->voter->get_age()<65) middle_aged++;
        else elders++;
        temp=temp->next;
    }
    return {young_adults,middle_aged,elders};
}

bool Election::register_candidate(Candidate* candidate) {
    if(candidate->get_age()>=35){
        Candidates* newNode = new Candidates();
        newNode->candidate = candidate;
        newNode->candidate->refValidity()=true;
        newNode->next = nullptr;
        if(this->headC == nullptr) this->headC = newNode;
        else{
            Candidates* temp = this->headC;
            while(temp->next){
                temp = temp->next;
            }
            temp->next = newNode;
        }
        return true;
    }
    return false;
}

ostream& operator<<(ostream& os, const Candidate& candidate) {
    os <<"\n"
       << "Candidate Name: " << candidate.get_name() << "\n"
       << "Age: " << candidate.get_age()<< "\n"
       << "Voivodship: " << candidate.get_voivodship() << "\n"
       << "Support: " << candidate.support << "\n";
    return os;
}

void Election::display_registered_candidates(){
    Candidates* temp=this->headC;
    while(temp){
        cout<<*(temp->candidate);
        temp=temp->next;
    }
}

void Election::distribute_candidates_to_voivodships(){
    Candidates* temp=this->headC;
    while(temp){
        Candidate* candidate=temp->candidate;
        for(auto& voivodship: voivodships){
            voivodship->register_candidate(candidate);
        }
        temp=temp->next;
    }
}

void Voivodship::register_candidate(Candidate* candidate){
    if(std::find(this->localVotes.begin(),this->localVotes.end(),candidate)==this->localVotes.end()){
        this->localVotes.push_back(candidate);
    }
}

unsigned int& Election::refAttendance(){
    
}

Voter::Voter(const char* name, const unsigned int age,const char* voivodship,bool vote,const bool validity){
    this->name=new char[strlen(name)+1];
    strcpy(this->name,name);
    this->age=age;
    this->voivodship=new char[strlen(voivodship)+1];
    strcpy(this->voivodship,voivodship);
    this->vote=vote;
    this->validity=validity;
}

Voter::~Voter(){
    delete[] this->name;
    delete[] this->voivodship;
}

void Voter::submit_vote(Candidate& candidate){
    if(this->validity && !this->vote){
        candidate.ref_support()++;
        this->has_voted()=true;
        candidate.add_supporter(this);
    }
}

void Candidate::add_supporter(Voter* voter){
    Supporters* node=new Supporters();
    node->voter=voter;
    node->next=nullptr;
    Supporters* temp=this->headS;
    if(temp==nullptr){
        this->headS=node;
    }else {
        while(temp->next) temp=temp->next;
        temp->next=node;
    }
}

unsigned int Voter::get_age()const {return this->age;}

char* Voter::get_voivodship()const {return this->voivodship;}

bool& Voter::has_voted(){return this->vote;}

Candidate::Candidate(const char* name, const unsigned int age, const char* voivodship, const bool vote, const bool validity, const int support)
    : Voter(name, age, voivodship, vote, validity), support(support), headS(nullptr) {}

void Candidate::free_supporters(){
    Supporters* current=headS;
    while (current!=nullptr) {
        Supporters* temp=current;
        current=current->next;
        delete temp;
    }
    headS=nullptr;
}

bool& Voter::refValidity(){return this->validity;}

void Candidate::submit_vote(){
    if(this->refValidity() && !this->has_voted()){
        ref_support()++;
        this->has_voted()=true;
    }
}

char* Voter::get_name()const {return this->name;}

unsigned int& Candidate::ref_support(){return this->support;}

void Candidate::display_voters(){
    Supporters* temp=this->headS;
    while(temp){
        cout<<temp->voter;
        temp=temp->next;
    }
    if(this->refValidity() && this->has_voted()) cout<<this; //Case when president self-voted 
}

Voivodship::Voivodship(const char* name, const unsigned int citizens){
    this->name=new char[strlen(name)+1];
    strcpy(this->name,name);
    this->citizens=citizens;
    this->headV=nullptr;
}

Voivodship::~Voivodship(){
    this->localVotes.clear();
}

bool Voivodship::register_voter(Voter* voter){
    if(this->find(voter->get_name(),voter->get_age())==false && this->number_of_citizens()>=this->number_of_voters()+1){
        if(strcmp(voter->get_voivodship(),this->get_name())==0 && voter->get_age()>=18){
            Voters* node=new Voters();
            //node->voter=new Voter(*voter);
            node->voter=voter;
            node->voter->refValidity()=true;
            node->next=nullptr;
            Voters* temp=this->headV;
            if(temp==nullptr){
                this->headV=node;
            }else {
                while(temp->next) temp=temp->next;
                temp->next=node;
            }
            return true;
        }
    }
    return false;
}

Voter::Voter(const Voter& voter){
    this->name=new char[strlen(voter.name)+1];
    strcpy(this->name,voter.name);
    this->voivodship=new char[strlen(voter.voivodship)+1];
    strcpy(this->voivodship,voter.voivodship);
    this->age=voter.age;
    this->validity=voter.validity;
    this->vote=voter.vote;
}

char* Voivodship::get_name(){return this->name;}

void Voivodship::display_registered_voters(){
    if(this->headV==nullptr) cout<<"No registered voters for: "<<this->get_name()<<endl;
    Voters* temp=this->headV;
    while(temp){
        cout<<*(temp->voter);
        temp=temp->next;
    }
    cout<<endl;
}

bool Voivodship::find(const char* name, const unsigned int age) {
    Voters* temp=this->headV;
    while(temp){
        if(strcmp(temp->voter->get_name(), name)==0 && temp->voter->get_age()==age) {
            return true;
        }
        temp=temp->next;
    }
    return false;
}

void Voivodship::display_local_support(){
    double support;
    cout<<endl;
    cout<<"Support for candidates in "<<this->get_name()<<" voivodship"<<endl;
    for(auto& entry : this->localVotes){
        auto& candidate = entry;
        int vote = candidate->local_support(this->get_name());
        if(strcmp(this->get_name(), candidate->get_voivodship())==0){
            support=candidate->has_voted()?((vote+1)*100)/(this->number_of_voters()+1):(vote*100)/this->number_of_voters();
        }else{
            support=(vote*100)/this->number_of_voters();
        }
        cout<<candidate->get_name();
        cout<<": "<<support<<"%"<<endl;
    }
}

unsigned int Candidate::local_support(const char* voivodship){
    unsigned int counter=0;
    Supporters* temp=this->headS;
    while(temp){
        if(strcmp(temp->voter->get_voivodship(),voivodship)==0) counter++;
        temp=temp->next;    
    }
    return counter;
}

unsigned int Voivodship::number_of_voters(){
    unsigned int counter=0;
    Voters* temp=this->headV;
    while(temp){
        counter++;
        temp=temp->next;
    }
    return counter;
}

unsigned int Voivodship::number_of_citizens(){
    return this->citizens;
}

ostream& operator<<(ostream& os,const Voter& voter){
    os << "Voter Name: " << voter.name << "\n"
       << "Age: " << voter.age<< "\n"
       << "Voivodship: " << voter.voivodship << "\n";
    return os;
}