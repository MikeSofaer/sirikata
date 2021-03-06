/*  Sirikata Kernel -- Task scheduling system
 *  DependencyTask.cpp
 *
 *  Copyright (c) 2008, Daniel Reiter Horn
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of Sirikata nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "util/Standard.hh"
#include "WorkQueue.hpp"
#include "DependencyTask.hpp"
namespace Sirikata {
namespace Task {

class DependentTask::CallSuccess : public WorkItem {
    DependentTask*mDependentTask;
  public:
    CallSuccess(DependentTask*dt){
        mDependentTask=dt;
    }

    virtual void operator()() {
        assert (mDependentTask->mNumThisWaitingOn==0);
        {
            AutoPtr deleteMe(this);
            (*mDependentTask)();
        }
    }
};

class DependentTask::CallFailed : public WorkItem {
    DependentTask*mDependentTask;
  public:
    CallFailed(DependentTask *dt) {
        mDependentTask=dt;
    }

    virtual void operator()() {
        assert (mDependentTask->mNumThisWaitingOn==0);
        {
            AutoPtr deleteMe(this);
            mDependentTask->finish(false);
        }
    }
};

void DependentTask::go() {
    if (mNumThisWaitingOn==0) {
        if (mFailure) {
            mWorkQueue->enqueue(new CallFailed(this));
        }else {
            mWorkQueue->enqueue(new CallSuccess(this));
        }
    }
}
void DependentTask::addDepender(DependentTask *depender) {
    ++depender->mNumThisWaitingOn;
    mDependents.push(depender);
}
void DependentTask::finish(bool success) {
    LockFreeQueue<DependentTask*>::NodeIterator depsIter(mDependents);
    DependentTask** deps;
    while ((deps = depsIter.next())!=NULL) {
        assert((*deps)->mNumThisWaitingOn!=0);
        if (!success) {
            (*deps)->mFailure=true;
        }
        --(*deps)->mNumThisWaitingOn;
        (*deps)->go();
    }
    delete this;
}
DependentTask::DependentTask(WorkQueue *queue)
	: mWorkQueue(queue), mNumThisWaitingOn(0), mFailure(false) {
}

DependentTask::~DependentTask(){}

}
}
