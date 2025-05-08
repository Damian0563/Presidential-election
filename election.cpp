#include <iostream>
#include "classes.h"
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

Election::Election(){
    this->headC=nullptr;
}

bool Election::add_voivodship(const char* voivodship_name, const unsigned int citizens){
    for (auto& v : this->voivodships) {
        if (strcmp(v->get_name(), voivodship_name) == 0) {
            return false;
        }
    }
    Voivodship* node=new Voivodship(voivodship_name,citizens);
    this->voivodships.push_back(node);
    return true;
}

Election::~Election(){
    if(this==nullptr) return;
    Candidates* current = this->headC;
    while (current != nullptr) {
        Candidates* temp = current;
        current = current->next;
        if(temp->candidate) delete temp->candidate;
        delete temp;         
    }
    this->headC = nullptr;
}

void Election::determine_winner(){
    if(headC!=nullptr){
        Candidates* temp=this->headC;
        double max=0,curr_support=0,total=0;
        Candidate* winner=this->headC->candidate;
        while(temp){
            if(temp->candidate->get_id()!=0){
                curr_support=temp->candidate->ref_support();
                total+=curr_support;
                if(max<curr_support){
                    max=curr_support;
                    winner=temp->candidate;
                }
            }
            temp=temp->next;
        }
        temp=this->headC;
        double percantage;
        cout<<endl<<"Winner: "<<winner->get_name()<<" of ID: "<<winner->get_id()<<" gets "<<(max*100)/total<<"%"<<" support in general election."<<endl;
        while(temp){
            if(temp->candidate->get_id()!=0 && winner->get_id()!=temp->candidate->get_id()){
                percantage=((temp->candidate->ref_support())*100)/total;
                cout<<temp->candidate->get_name()<<" of ID: "<<temp->candidate->get_id()<<", support: "<<percantage<<"%"<<endl;
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
            if(temp->candidate->get_id()!=0){
                total=temp->candidate->ref_support();
                vector<int> age_dist=temp->candidate->age_distribution();
                young_adults=age_dist[0];
                middle_aged=age_dist[1];
                elders=age_dist[2];
                
                cout<<temp->candidate->get_name()<<" with ID "<<temp->candidate->get_id()<<": "<<endl;
                cout<<"\tYoung adults: "<<(young_adults*100)/total<<"%"<<endl;
                cout<<"\tMiddle aged: "<<(middle_aged*100)/total<<"%"<<endl;
                cout<<"\tElders: "<<(elders*100)/total<<"%"<<endl;
            }
            temp=temp->next;
        }
    }else cout<<"No candidates registered, can not display support by age group"<<endl;
}

void Election::display_registered_candidates(){
    Candidates* temp=this->headC;
    if(!temp) cout<<"No candidates registered"<<endl;
    else cout<<endl<<"Registered candidates: "<<endl;
    while(temp){
        if(temp->candidate->get_id()!=0) cout<<*(temp->candidate);
        temp=temp->next;
    }
}

double Election::election_attendance(){
    double population=0;
    double voters=0;
    Candidates* temp=this->headC;
    for(auto& voivodship : this->voivodships){
        population+=voivodship->number_of_citizens();
        voters+=voivodship->number_of_submitted_votes();
        while(temp){
            if(temp->candidate->get_id()!=0 && strcmp(temp->candidate->get_voivodship(),voivodship->get_name())==0){
                if(temp->candidate->has_voted()){
                    voters++;
                }
                population++;
            }
            temp=temp->next;
        }
        temp=this->headC;
    }
    cout<<endl<<"Total number of citizens: "<<population<<endl;
    cout<<"Total number of voters: "<<voters<<endl;
    double percantage=(voters*100)/population;
    return percantage;
}

unsigned int Election::generate_id(){
    static unsigned int last_id = 1; // Keeps track of the last assigned ID
    return ++last_id; // Increment and return a unique ID
}

Voter* Election::register_voter(const char* name, const unsigned int age,const char* voivodship_name){
    if(this->voivodships.empty()) return nullptr; //No voivodships registered
    for(auto& v:this->voivodships){
        if(strcmp(v->get_name(),voivodship_name)==0 && v->number_of_citizens()>v->number_of_voters()){ //Ensure no duplicate citizens with the sme name and age
            if(age>=18){
                unsigned int id=this->generate_id();;
                Voter* node=new Voter(id,name,age,voivodship_name);
                v->add_voter(node);
                return node;
            }
            return nullptr;
        }
    }
    return nullptr;
}

bool Election::find_candidate(const unsigned int id){
    if(id==0) return false;
    Candidates* temp=this->headC;
    while(temp){
        if(temp->candidate->get_id()==id) return true;
        temp=temp->next;
    }
    return false;
}

Candidate* Election::register_candidate(const char* name, const unsigned int age, const char* voivodship_name){
    for(auto& v:this->voivodships){
        if(strcmp(v->get_name(),voivodship_name)==0 && v->number_of_citizens()<=v->number_of_voters()){ //Candidate can not be registered if the number of citizens is less than the number of voters
            return nullptr; 
        }else if(strcmp(v->get_name(),voivodship_name)==0 && age>=35){
            v->decrease_voter_count(); //Decrement the number of citizens
        }
    }
    if(age>=35){
        unsigned int id=this->generate_id();
        Candidate* node=new Candidate(id,name,age,voivodship_name);
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
                if(temp->candidate->get_id()!=0){
                    unsigned int increment1=0,increment2=this->get_votes_of_candidates(voivodship_name);
                    if(strcmp(temp->candidate->get_voivodship(),voivodship_name)==0 && temp->candidate->has_voted()){
                        increment1++;
                    }
                    //cout<<temp->candidate->supporters_in_voivodship(voivodship_name)<<"+"<<increment1<<", "<<v->number_of_submitted_votes()<<"+"<<increment2<<endl;
                    cout<<temp->candidate->get_name()<<" of ID "<<temp->candidate->get_id()<<": "<<(double(temp->candidate->supporters_in_voivodship(voivodship_name)+increment1)*100)/double(v->number_of_submitted_votes()+increment2)<<"%"<<endl;
                }
                temp=temp->next;
            }
            return true;
        }
    }
    return false;
}

unsigned int Election::get_votes_of_candidates(const char* voivodship_name){
    for(auto& v : this->voivodships){
        if(strcmp(v->get_name(),voivodship_name)==0){
            Candidates* temp=this->headC;
            int counter=0;
            while(temp){
                if(strcmp(temp->candidate->get_voivodship(),v->get_name())==0 && temp->candidate->has_voted()){
                    counter++;
                }
                temp=temp->next;
            }
            return counter;
        }
    }
    return 0;
}

bool Election::local_support(const Candidate* candidate,const char* voivodship_name){
    for(auto& v : this->voivodships){
        if(strcmp(v->get_name(),voivodship_name)==0){
            cout<<endl<<v->get_name()<<": "<<endl;
            Candidates* temp=this->headC;
            while(temp){
                if(strcmp(temp->candidate->get_name(),candidate->get_name())==0){
                    int  increment1=0;
                    if(strcmp(temp->candidate->get_voivodship(),voivodship_name)==0){
                        increment1++;
                    }
                    int increment2=this->get_votes_of_candidates(voivodship_name);
                    //cout<<temp->candidate->supporters_in_voivodship(voivodship_name)<<"+"<<increment1<<", "<<v->number_of_submitted_votes()<<"+"<<increment2<<endl;
                    cout<<temp->candidate->get_name()<<": "<<(double(temp->candidate->supporters_in_voivodship(voivodship_name)+increment1)*100)/double(v->number_of_submitted_votes()+increment2)<<"%"<<endl;
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

Voter* Election::get_voter_node(unsigned int id){
    for(auto& v : this->voivodships){
        if(v->get_voter(id)!=nullptr) return v->get_voter(id);
    }
    return nullptr;
}

void Election::die_voter(unsigned int voter_id){
    Voter* voter=this->get_voter_node(voter_id);
    for(auto& v:this->voivodships){
        if(v->find(voter_id) && !voter->has_voted()) v->decrease_voter_count();
    }
    if(voter) voter->refId()=0;
}

void Election::die_candidate(unsigned int candidate_id){
    if(!this->find_candidate(candidate_id)) return;
    Candidates* temp=this->headC;
    while(temp){
        if(temp->candidate->get_id()==candidate_id){
            temp->candidate->refId()=0;
        }
        temp=temp->next;
    }
}

unsigned int Election::get_number_of_citizens(const char* voivodship_name){
    for(auto& v : this->voivodships){
        if(strcmp(v->get_name(),voivodship_name)==0){
            return v->number_of_citizens();
        }
    }
    return 0;
}

Candidate* Election::get_candidate_node(unsigned int id){
    Candidates* temp=this->headC;
    while(temp){
        if(temp->candidate->get_id()==id) return temp->candidate;
        temp=temp->next;
    }
    return nullptr;
}
