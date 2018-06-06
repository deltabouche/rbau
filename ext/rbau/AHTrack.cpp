/*
 *  AHTrack.cpp
 *
 *  Created by simon on 10/03/09.
 *  Modified by Simon on march 16th 2010
 *
 */


#include "AHTrack.h"

#include <iostream>

using namespace std;

AHTrack::AHTrack(CAComponentDescription synthDescription, AHGraph* graph, int trackIndex)
{
    graph_ = graph;
    synth_ = graph_->AddAHAudioUnitToGraph(synthDescription);
    trackIndex_ = trackIndex;
    armed_ = false;
    ConnectAllNodes();
    graph_->UpdateGraph();
}

AHTrack::~AHTrack()
{
    DisconnectAllNodes();
    graph_->RemoveAHAudioUnitFromGraph(synth_);
    for (list<AHAudioUnit*>::iterator it = effects_.begin(); it != effects_.end(); it++)
        graph_->RemoveAHAudioUnitFromGraph(*it);
    graph_->UpdateGraph();
    //CAShow(graph_->GetAUGraph());
}

AHAudioUnit* AHTrack::SetSynth(const CAComponentDescription desc)
{
    graph_->DisconnectMixerInputs(trackIndex_);
	DisconnectAllNodes();

    graph_->RemoveAHAudioUnitFromGraph(synth_);
		
	synth_ = graph_->AddAHAudioUnitToGraph(desc);
	
	ConnectAllNodes();
    graph_->ConnectMixerInputs(trackIndex_);
    
    graph_->UpdateGraph();
	
	return synth_;
}

AHAudioUnit* AHTrack::SetSynth(const string name, const string manu)
{
    CAComponentDescription desc;
    if (FindAudioUnitFromName(name, manu, desc))
    {
        return SetSynth(desc);
    }
    else
    {
        cout << "\nThe audio unit '" << name;
        if (!manu.empty())
            cout << "' - '" << manu;
        cout << "' could not be found";
        return NULL;
    }
}

AHAudioUnit* AHTrack::AddEffect(const CAComponentDescription desc, int index)
{
    graph_->DisconnectMixerInputs(trackIndex_);
	DisconnectAllNodes();
	
    AHAudioUnit* eff = graph_->AddAHAudioUnitToGraph(desc);
    
    if (index==-1)
        effects_.push_back(eff);
    else
    {
        list<AHAudioUnit*>::iterator it = effects_.begin();
        for (int i=0; i<index; i++)
            it++;
        effects_.insert(it, eff);
    }
	
	ConnectAllNodes();
    graph_->ConnectMixerInputs(trackIndex_);
	
    graph_->UpdateGraph();
	return eff;
}

AHAudioUnit* AHTrack::AddEffect(const string name , const string manu, int index)
{
    CAComponentDescription desc; 
    if (FindAudioUnitFromName(name, manu, desc))
    {
        return AddEffect(desc, index);
    }
    else
    {
        cout << "\nThe audio unit '" << name;
        if (!manu.empty())
            cout << "' - '" << manu;
        cout << "' could not be found";
        return NULL;
    }
}

void AHTrack::RemoveEffectAt(int index)
{
    // TODO : check if there is and effect at index "index" before trying to remove it!
    graph_->DisconnectMixerInputs(trackIndex_);
	DisconnectAllNodes();
    list<AHAudioUnit*>::iterator it = effects_.begin();
    for (int i=0; i<index; i++)
        it++;
    
    graph_->RemoveAHAudioUnitFromGraph(*it);
    
    effects_.erase(it);
    
	ConnectAllNodes();
    graph_->ConnectMixerInputs(trackIndex_);
    
    graph_->UpdateGraph();
}


void AHTrack::RemoveLastEffect()
{
    if (!effects_.empty())
    {
        graph_->DisconnectMixerInputs(trackIndex_);
        DisconnectAllNodes();

        graph_->RemoveAHAudioUnitFromGraph(effects_.back());
        
        
        effects_.pop_back();
        
        ConnectAllNodes();
        graph_->ConnectMixerInputs(trackIndex_);
        
        graph_->UpdateGraph();
    }
}

void AHTrack::ConnectAllNodes() const
{
    if(effects_.size())
	{
		list<AHAudioUnit*>::const_iterator prev_it = effects_.begin(), it = effects_.begin();
		it++;
		
		PrintIfErr( AUGraphConnectNodeInput(graph_->GetAUGraph(),
                                            synth_->GetAUNode(),
                                            0,
                                            (*prev_it)->GetAUNode(),
                                            0) );

        
        if (effects_.size() > 1 )
        {
            while(it != effects_.end())
            {
                PrintIfErr( AUGraphConnectNodeInput(graph_->GetAUGraph(), (*prev_it)->GetAUNode(), 0, (*it)->GetAUNode(), 0) );
                it++;
                prev_it++;
            }
        }
    }
}

void AHTrack::DisconnectAllNodes() const
{
    if(!effects_.size()) return;

    for ( list<AHAudioUnit*>::const_iterator it = effects_.begin(); it != effects_.end(); it++ )
    {
        PrintIfErr( AUGraphDisconnectNodeInput( graph_->GetAUGraph(), (*it)->GetAUNode(), 0 ) );
    }
}

/*AHAudioUnit* AHTrack::GetEffectAt(int index)
{
	list<AHAudioUnit*>::iterator it = effects_.begin();
	for(int i=0; i<index; i++)
		it++;
	return &(*it);
}*/