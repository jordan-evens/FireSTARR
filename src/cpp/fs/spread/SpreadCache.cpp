/* SPDX-License-Identifier: AGPL-3.0-or-later */
#include "SpreadCache.h"
#include "../log/Log.h"
#include "../sim/Scenario.h"
#include "FireSpread.h"
namespace fs
{
static MathSize find_min_ros(const Scenario& scenario, const DurationSize time)
{
  // HACK: resolve once and fail if not set already
  static const auto& settings = fs::settings::instance();
  const MathSize min_ros = settings.minimum_ros;
  return settings.deterministic ? min_ros : std::max(scenario.spreadThresholdByRos(time), min_ros);
}
ptr<const SpreadInfo> SpreadCache::add_spread(
  ptr<const Scenario> scenario,
  const XYIdx& loc,
  DurationSize time
) noexcept
{
  const Cell for_cell = scenario->cell(loc);
  const auto key = for_cell.key();
  const auto wx = scenario->weather(loc, time);
  const auto wx_daily = scenario->weather_daily(loc, time);
  const auto k = std::pair{key, wx};
  return &(spread_info_
             .try_emplace(
               k,
               time,
               find_min_ros(*scenario, time),
               scenario->cellSize(),
               key,
               scenario->nd(time),
               wx,
               wx_daily
             )
             .first->second);
}
const OffsetSet& SpreadCache::offsets(const SpreadCacheKey& key) const noexcept
{
  return spread_info_.at(key).offsets();
}
MathSize SpreadCache::maxIntensity(const SpreadCacheKey& key) const noexcept
{
  auto seek_spread = spread_info_.find(key);
  const auto max_intensity =
    (spread_info_.end() == seek_spread) ? RoundedIntensity{0} : seek_spread->second.maxIntensity();
  return max_intensity;
}
}
