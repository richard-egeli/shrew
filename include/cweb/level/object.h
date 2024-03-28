#ifndef INCLUDE_CWEB_LEVEL_OBJECT_H_
#define INCLUDE_CWEB_LEVEL_OBJECT_H_

typedef void (*object_update_func)(void);
typedef void (*object_render_func)(void);

void object_render_add(object_render_func func);

void object_update_add(object_update_func func);

void object_update(void);

void object_render(void);

#endif  // INCLUDE_CWEB_LEVEL_OBJECT_H_
