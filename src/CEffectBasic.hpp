/**
 * @author Jakub Kučera <kucerj56@fit.cvut.cz>
 * @date 25.05.2020
 */

#ifndef TOWERDEFENCE_CEFFECTBASIC_HPP
#define TOWERDEFENCE_CEFFECTBASIC_HPP


#include "CEffect.hpp"
/// Basic effect. Only gives damage once and doesn't transfer effect to enemies.
class CEffectBasic : public CEffect {
public:
    CEffectBasic(int damage, int range, int towerId, bool zoneAll)
            : CEffect(damage, range, 0, 0, 0, towerId, zoneAll) {}
};


#endif //TOWERDEFENCE_CEFFECTBASIC_HPP
