#include "classes.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

Election::Election(){
    this->headC=nullptr;
}

bool Election::add_voivodship(const char* name, const unsigned int citizens){
    for (auto& v : this->voivodships) {
        if (strcmp(v->get_name(), name) == 0) {
            return false;
        }
    }
    Voivodship* node=new Voivodship(name,citizens);
    this->voivodships.push_back(node);
    return true;
}

Election::~Election(){
    this->headC=nullptr;
}

void Election::determine_winner(){
    if(headC!=nullptr){
        Candidates* temp=this->headC;
        double max=0,curr_support=0,total=0;
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
        cout<<endl<<"Winner: "<<winner->get_name()<<" gets "<<(max*100)/total<<"%"<<" support in general election."<<endl;
        while(temp){
            if(strcmp(winner->get_name(),temp->candidate->get_name())!=0){
                percantage=((temp->candidate->ref_support())*100)/total;
                cout<<temp->candidate->get_name()<<", support: "<<percantage<<"%"<<endl;
            }
            temp=temp->next;
        }
        cout<<"Election attendance: "<<this->election_attendance()<<"%"<<endl;
    }else cout<<endl<<"No candidates registered"<<endl;
}

void Election::support_by_age_group(){
    if(this->headC){
        Candidates* temp=this->headC;
        double young_adults=0,middle_aged=0,elders=0;
        double total=0;
        if(!temp) cout<<temp->candidate->get_name()<<" has no supporters"<<endl;
        else cout<<endl<<"Support by age group for candidates: "<<endl;
        while(temp){
            total=temp->candidate->ref_support();
            young_adults=temp->candidate->age_distribution()[0];
            middle_aged=temp->candidate->age_distribution()[1];
            elders=temp->candidate->age_distribution()[2];
            if(temp->candidate->has_voted()){
                if(temp->candidate->get_age()<40) young_adults++;
                else if(temp->candidate->get_age()<=65) middle_aged++;
                else elders++;
            } 
            cout<<temp->candidate->get_name()<<": "<<endl;
            cout<<"\tYoung adults: "<<(young_adults*100)/total<<"%"<<endl;
            cout<<"\tMiddle aged: "<<(middle_aged*100)/total<<"%"<<endl;
            cout<<"\tElders: "<<(elders*100)/total<<"%"<<endl;
            temp=temp->next;
        }
    }else cout<<"No candidates registered, can not display support by age group"<<endl;
}

void Election::display_registered_candidates(){
    Candidates* temp=this->headC;
    if(!temp) cout<<"No candidates registered"<<endl;
    else cout<<endl<<"Registered candidates: "<<endl;
    while(temp){
        cout<<*(temp->candidate);
        temp=temp->next;
    }
}

double Election::election_attendance(){
    double population=0;
    double voters=0;
    for(auto& voivodship : this->voivodships){
        population+=voivodship->number_of_citizens();
        voters+=voivodship->number_of_voters();
    }
    double percantage=(voters*100)/population;
    return percantage;
}

Voter* Election::register_voter(const char* name, const unsigned int age,const char* voivodship){
    for(auto& v:this->voivodships){
        if(strcmp(v->get_name(),voivodship)==0 && !v->find(name,age) && v->number_of_citizens()>v->number_of_voters()){ //Ensure no duplicate citizens with the sme name and age
            if(age>=18){
                Voter* node=new Voter(name,age,voivodship);
                v->add_voter(node);
                return node;
            }
            return nullptr;
        }
    }
    return nullptr;
}

Candidate* Election::register_candidate(const char* name, const unsigned int age, const char* voivodship){
    for(auto& v:this->voivodships){
        if(strcmp(v->get_name(),voivodship)==0 && v->number_of_citizens()<=v->number_of_voters()){ //Candidate can not be registered if the number of citizens is less than the number of voters
            return nullptr; 
        }
    }
    if(age>=35){
        Candidate* node=new Candidate(name,age,voivodship);
        Candidates* temp=this->headC;
        if(!temp) {
            this->headC = new Candidates();
            this->headC->candidate = node;
            this->headC->next = nullptr;
            return node;
        }
        while(temp->next){
            temp=temp->next;
        }
        temp->next = new Candidates();
        temp->next->candidate = node;
        temp->next->next = nullptr;
        return node;
    }
    return nullptr;
}

void Election::display_all_voters(){
    for(auto& v : this->voivodships){
        v->display_voters();
    }
}

bool Election::display_registered_voters(const char* voivodship_name){
    for(auto& v : this->voivodships){
        if(strcmp(v->get_name(),voivodship_name)==0){
            v->display_voters();
            cout<<"Number of voters: "<<v->number_of_voters()<<", Number of citizens: "<<v->number_of_citizens()<<endl;
            return true;
        }
    }
    return false;
}

bool Election::display_local(const char* voivodship_name){
    for(auto& v : this->voivodships){
        if(strcmp(v->get_name(),voivodship_name)==0){
            cout<<endl<<v->get_name()<<": "<<endl;
            Candidates* temp=this->headC;
            while(temp){
                cout<<temp->candidate->get_name()<<": "<<(double(temp->candidate->supporters_in_voivodship(v->get_name()))*100)/double(v->number_of_voters())<<"%"<<endl;
                temp=temp->next;
            }
            return true;
        }
    }
    return false;
}

bool Election::local_support(const Candidate* candidate,const char* voivodship_name){
    for(auto& v : this->voivodships){
        if(strcmp(v->get_name(),voivodship_name)==0){
            cout<<endl<<v->get_name()<<": "<<endl;
            Candidates* temp=this->headC;
            while(temp){
                if(strcmp(temp->candidate->get_name(),candidate->get_name())==0){
                    cout<<temp->candidate->get_name()<<": "<<(double(temp->candidate->supporters_in_voivodship(v->get_name()))*100)/double(v->number_of_voters())<<"%"<<endl;
                    return true;
                }
                temp=temp->next;
            }
        }
    }
    return false;
}

void Election::display_voters(const Candidate* candidate){
    candidate->display_supporters();
}

void Election::display_voivodships(){
    for(auto& v : this->voivodships){
        cout<<v->get_name()<<", ";
    }
    cout<<endl;
}



ostream& operator<<(ostream& os, const Candidate& candidate) {
    os <<"\n"
       << "Candidate Name: " << candidate.get_name() << "\n"
       << "Age: " << candidate.get_age()<< "\n"
       << "Voivodship: " << candidate.get_voivodship() << "\n"
       << "Support: " << candidate.support << "\n";
    return os;
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

Candidate::Candidate(const char* name, const unsigned int age, const char* voivodship, const bool vote, const int support)
    : Voter(name, age, voivodship, vote), support(support), headS(nullptr) {}

void Candidate::free_supporters(){
    Supporters* current=this->headS;
    while (current!=nullptr) {
        Supporters* temp=current;
        current=current->next;
        delete temp->voter;
        delete temp;
    }
    this->headS=nullptr;
}

void Candidate::submit_vote(){
    ref_support()++;
    this->has_voted()=true;
}

unsigned int& Candidate::ref_support(){return this->support;}

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

Candidate::~Candidate(){
    this->free_supporters();
}

void Candidate::display_supporters() const {
    Supporters* temp=this->headS;
    cout<<endl<<this->get_name()<<" supporters: "<<endl;
    while(temp){
        cout<<*(temp->voter);
        temp=temp->next;
    }
}

unsigned int Candidate::supporters_in_voivodship(const char* voivodship){
    unsigned int counter=0;
    Supporters* temp=this->headS;
    while(temp){
        if(strcmp(temp->voter->get_voivodship(),voivodship)==0) counter++;
        temp=temp->next;
    }
    return counter;
}


unsigned int Voter::get_age()const {return this->age;}

char* Voter::get_voivodship()const {return this->voivodship;}

bool& Voter::has_voted(){return this->vote;}

ostream& operator<<(ostream& os,const Voter& voter){
    os << "Voter Name: " << voter.name << "\n"
       << "Age: " << voter.age<< "\n"
       << "Voivodship: " << voter.voivodship << "\n";
    return os;
}

char* Voter::get_name()const {return this->name;}

Voter::Voter(const char* name, const unsigned int age,const char* voivodship,bool vote){
    this->name=new char[strlen(name)+1];
    strcpy(this->name,name);
    this->age=age;
    this->voivodship=new char[strlen(voivodship)+1];
    strcpy(this->voivodship,voivodship);
    this->vote=vote;
}

Voter::~Voter(){
    delete[] this->name;
    delete[] this->voivodship;
}

void Voter::submit_vote(Candidate& candidate){
    
    candidate.ref_support()++;
    this->has_voted()=true;
    candidate.add_supporter(this);
    
}





Voivodship::Voivodship(const char* name, const unsigned int citizens){
    this->name=new char[strlen(name)+1];
    strcpy(this->name,name);
    this->citizens=citizens;
    this->headV=nullptr;
}

Voivodship::~Voivodship(){
    this->free_voters();
}

char* Voivodship::get_name(){return this->name;}

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

void Voivodship::free_voters(){
    Voters* current=this->headV;
    while (current!=nullptr) {
        Voters* temp=current;
        current=current->next;
        delete temp->voter;
        delete temp;
    }
    this->headV=nullptr;
}

void Voivodship::display_voters(){
    Voters* temp=this->headV;
    cout<<endl<<this->name<<" voters: "<<endl;
    while(temp){
        cout<<*(temp->voter);
        temp=temp->next;
    }
}

bool Voivodship::add_voter(Voter* voter){
    Voters* temp = this->headV;
    if(this->find(voter->get_name(), voter->get_age())) {
        return false;
    }
    if(this->number_of_citizens()<=this->number_of_voters()){
        return false;
    }
    Voters* node = new Voters();
    node->voter = voter;
    node->next = this->headV;
    this->headV = node;
    return true;
}