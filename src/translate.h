/*
 * translate.h
 *
 *  Created on: 06.06.2016
 *      Author: stefan
 */

#ifndef TRANSLATE_H_
#define TRANSLATE_H_

#include <engine/utils/localisation.h>

//#define _(STRING) gettext(STRING)
#include <string>

#define _(STRING) Localisation::Instance().translate(STRING)

#endif /* TRANSLATE_H_ */
