/*  Sirikata Utilities -- Math Library
 *  TemporalValue.hpp
 *
 *  Copyright (c) 2009, Daniel Reiter Horn
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
#ifndef _TEMPORAL_VALUE_HPP_
#define _TEMPORAL_VALUE_HPP_

namespace Sirikata {

template <typename Value, typename UpdatePredicate> class TemporalValue
    : protected UpdatePredicate {//inherit from update predicate since 0 sized inheritance takes 0 space
    Value mCurrentValue;
    Time mWhen;
public:
    TemporalValue(const Time &when,
                  const Value&l,
                  const UpdatePredicate &needsUpdate):
       UpdatePredicate(needsUpdate),
       mCurrentValue(l),
       mWhen(when){}
    bool needsUpdate(const Time&now, const Value&actualValue) const{
        const UpdatePredicate * mNeedsUpdate=this;
        return (*mNeedsUpdate)(actualValue,extrapolate(now));
    }
    Value extrapolate(const Time &t) const {
        return mCurrentValue.extrapolate(t-mWhen);
    }
    Time getLastValueUpdateTime() const{
        return mWhen;
    }
    TemporalValue<Value,UpdatePredicate>& updateValue(const Time&t,const Value&l) {
        mCurrentValue=l;
        mWhen=t;
        return *this;
    }
};
}
#endif