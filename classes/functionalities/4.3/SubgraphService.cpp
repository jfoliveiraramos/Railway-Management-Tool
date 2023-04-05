//
// Created by joaof on 04/04/2023.
//

#include "SubgraphService.h"
#include "../4.1/BasicServices.h"


SubgraphService::SubgraphService(Graph *graph): originalGraph(graph){
    resetSubgraph();
}

void SubgraphService::resetSubgraph() {
    *subGraph = Graph(*originalGraph);
}

void SubgraphService::deleteStation(int id) {

    for (Edge* e: subGraph->findVertex(id)->getAdj())
        subGraph->findVertex(e->getDest()->getId())->removeEdge(id);

    subGraph->removeVertex(id);
}

void SubgraphService::deleteEdge(int orig, int dest) {

    Vertex* o = subGraph->findVertex(orig);
    o->removeEdge(dest);

    Vertex* d = subGraph->findVertex(dest);
    d->removeEdge(orig);
}

void SubgraphService::deleteService(ServiceType serviceType) {

    for (Vertex* v: subGraph->getVertexSet()){
        for (Edge* e: v->getAdj()){
            if (e->getService() == serviceType){
                v->removeEdge(e->getDest()->getId());
            }
        }
    }
}

int SubgraphService::maxFlow(int source, int target) {

    return BasicServices(subGraph).maxFlow(source, target);
}

std::vector<Vertex*> SubgraphService::mostAffectedStations(int orig, int dest){

    resetSubgraph();

    BasicServices basicServices(subGraph);

    std::map<Vertex*, double> og_averages, new_averages;

    for(Vertex* s: subGraph->getVertexSet()){

        double maxFlowSum = 0;
        int count = 0;
        for(Vertex* t: subGraph->getVertexSet()){
            if (s == t)
                continue;
            maxFlowSum += basicServices.maxFlow(s->getId(), t->getId());
            count++;
        }
        og_averages.insert(std::make_pair(s, maxFlowSum/count));
    }

    deleteEdge(orig, dest);

    for(Vertex* s: subGraph->getVertexSet()){

        double maxFlowSum = 0;
        int count = 0;
        for(Vertex* t: subGraph->getVertexSet()){
            if (s == t)
                continue;
            maxFlowSum += basicServices.maxFlow(s->getId(), t->getId());
            count++;
        }
        new_averages.insert(std::make_pair(s, maxFlowSum/count));
    }

    int maxDiff = INT_MIN;
    std::vector<Vertex*> affected;

    for(Vertex* s: subGraph->getVertexSet()) {

        int diff = og_averages[s] - new_averages[s];
        if (diff > maxDiff)
            affected = {s};
        else if (diff == maxDiff)
            affected.push_back(s);
    }

    return affected;
}