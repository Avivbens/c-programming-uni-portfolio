#include "../../cli/cli-parser.h"
#include "../../common/command.h"
#include "../../common/register.h"
#include "../../constants/constants.h"
#include "../../data/macros-data.h"
#include "../../data/symbols-data.h"
#include "../../utils/array/array.h"
#include "../../utils/number/number.h"
#include "../../utils/string/string.h"

#ifndef LABELS_PARSER_H
#define LABELS_PARSER_H

LabelType is_label(String line);
void *handle_labels(String *file_names);

#endif
