#ifndef VALUES_INSERTER_H_LYZDHLNG
#define VALUES_INSERTER_H_LYZDHLNG

#include "converter_core.h"

namespace mfast
{
  namespace SQLite3
  {

    class aggregate_inserter
    {
    public:
      aggregate_inserter(const converter_core& core);

      field_cref insert(const aggregate_cref& ref, const field_cref& foreign_key);

    private:
      const converter_core& core_;
      value_storage storage_;
    };

  } /* SQLite3 */

} /* mfast */

#endif /* end of include guard: VALUES_INSERTER_H_LYZDHLNG */
