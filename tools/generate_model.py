import json

data = open("models/ja.json").read()
model = json.loads(data)

header = """#include "models/JAModel.h"
#include "models/model.h"

namespace budoux {
    const Model& GetJAModel() {
        static Model model {
            .params = {{"""

footer = """
 },
                
            }
        };
        return model;
    }
}
"""

keys = list(model.keys())
keys.sort()
# print(model)

params_str = ""

for x in keys:
    m = model[x]
    params = []
    for pair in m.items():
        params.append('{{std::string("{0}"), {1}}}'.format(*pair))

    params_str += "// param: {0}\n{{ {1} }},\n".format(x, ",".join(params))
    

    

print(header)
print(params_str)
print(footer)

