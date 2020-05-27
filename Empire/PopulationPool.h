#ifndef _PopulationPool_h_
#define _PopulationPool_h_

#include "../util/Export.h"

#include <boost/signals2/signal.hpp>

#include <vector>

/** The PopulationPool class keeps track of an empire's total population and its growth. */
class FO_COMMON_API PopulationPool {
public:
    PopulationPool();

    const std::vector<int>& PopCenterIDs() const { return m_pop_center_ids; }   ///< returns the PopCenter vector
    float Population() const;   ///< returns current total population

    /** emitted after updating population and growth numbers */
    mutable boost::signals2::signal<void ()> ChangedSignal;

    void    SetPopCenters(const std::vector<int>& pop_center_ids);
    void    Update();                           ///< recalculates total population and growth

private:
    std::vector<int>    m_pop_center_ids;       ///< UniverseObject ids of PopCenters that contribute to the pool
    float               m_population = 0.0f;    ///< total population of all PopCenters in pool

    template <typename Archive>
    friend void serialize(Archive&, PopulationPool&, unsigned int const);
};



#endif
