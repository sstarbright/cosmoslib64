#include "cosams.h"
#include <libdragon.h>

void stage_init(Stage* stage, int index_size) {
    stage->enabled = true;
    stage->visible = true;
    stage->ambient_color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF);
    stage->fog_color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF);
    stage->is_fog = false;
    stage->fog_start = 5.f;
    stage->fog_end = 10.f;
    
    uint32_t index_mem_size = sizeof(void*)*index_size;
    stage->actor = NULL;
    stage->camera = NULL;
    stage->light = NULL;
    stage->light_count = 0;
    stage->current_light = 0;
    stage->draw = NULL;
    stage->prev = stage;
    stage->next = stage;
    if (index_size > 0)
        stage->actor_table = malloc(index_mem_size);
    else
        stage->actor_table = NULL;
    memset(stage->actor_table, 0, index_mem_size);
}
void stage_add_actor(Stage* stage, Actor* actor, int slot) {
    Actor* first_actor = stage->actor;
    if (first_actor) {
        linked_add_to_list(first_actor->prev, first_actor, actor, offsetof(Actor, prev), offsetof(Actor, next));
    } else {
        stage->actor = actor;
    }
    actor->stage = stage;
    if (slot >= 0) {
        if (stage->actor_table) {
            if(!stage->actor_table[slot]) {
                stage->actor_table[slot] = actor;
                actor->index_slot = slot;
            } else {
                debugf("An Actor already exists in Slot %i!\n", slot);
            }
        } else {
            debugf("The Actor Table is missing.\n");
        }
    }
}
void stage_life(Stage* stage, float delta) {
    Actor* current_actor = stage->actor;
    do {
        if (current_actor->enabled) {
            actor_life(current_actor, delta);
            current_actor = current_actor->next;
        }
    }
    while(current_actor != stage->actor);
}
void stage_predraw(Stage* self, float delta, uint32_t matrix_id) {
    Render3DM* current_draw = self->draw;
    if (current_draw) {
        do {
            if (((Module*)current_draw)->enabled) {
                current_draw->predraw(current_draw, delta, matrix_id);
                current_draw = current_draw->next;
            }
        }
        while(current_draw != self->draw);
    }
}
void stage_draw(Stage* self, float delta, uint32_t matrix_id) {
    self->current_light = 0;
    
    // Draw Camera
    Render3DM* current_draw = (Render3DM*)self->camera;
    if (current_draw) {
        current_draw->draw(current_draw, delta, matrix_id);
    }

    t3d_screen_clear_depth();
    
    /*if (self->is_fog) {
        rdpq_mode_fog(RDPQ_FOG_STANDARD);
        rdpq_set_fog_color(self->fog_color);
    
        t3d_fog_set_range(self->fog_start, self->fog_end);
        t3d_fog_set_enabled(true);
    }*/
    
    t3d_light_set_count(self->light_count);

    // Draw Lights
    current_draw = self->light;

    if (current_draw) {
        do {
            if (((Module*)current_draw)->enabled) {
                current_draw->draw(current_draw, delta, matrix_id);
                current_draw = current_draw->next;
            }
        }
        while(current_draw != self->light);
    }

    // Draw Meshes
    current_draw = self->draw;

    if (current_draw) {
        do {
            if (((Module*)current_draw)->enabled) {
                current_draw->draw(current_draw, delta, matrix_id);
                current_draw = current_draw->next;
            }
        }
        while(current_draw != self->draw);
    }

    t3d_light_set_ambient(&self->ambient_color.r);
}
void stage_kill(Stage* stage) {
    linked_pop_from_list(stage, offsetof(Stage, prev), offsetof(Stage, next));
    stage_simple_kill(stage);
}
void stage_simple_kill(Stage* stage) {
    if (stage->actor) {
        linked_kill_list(stage->actor, actor_simple_kill, offsetof(Actor, prev), offsetof(Actor, next));
    }

    if (stage->actor_table)
        free(stage->actor_table);

    free(stage);
}

void actor_init(Actor* actor, int index_size) {
    uint32_t index_mem_size = sizeof(void*)*index_size;
    actor->attributes = 0;
    actor->index_size = index_size;
    actor->module = NULL;
    actor->stage = NULL;
    actor->enabled = true;
    actor->index_slot = -1;
    actor->prev = actor;
    actor->next = actor;
    if (index_size > 0)
        actor->module_table = malloc(index_mem_size);
    else
        actor->module_table = NULL;
    memset(actor->module_table, 0, index_mem_size);
}
void actor_add_module(Actor* actor, Module* module, int slot) {
    if (module) {
        Module* first_module = actor->module;
        if (first_module) {
            linked_add_to_list(first_module->prev, first_module, module, offsetof(Module, prev), offsetof(Module, next));
        } else {
            actor->module = module;
        }
        if (slot >= 0) {
            if (actor->module_table) {
                if(!actor->module_table[slot]) {
                    actor->module_table[slot] = module;
                    module->index_slot = slot;
                } else {
                    debugf("A Module already exists in Slot %i!\n", slot);
                }
            } else {
                debugf("The Module Table is missing.\n");
            }
        }
        module->actor = actor;
    }
}
void actor_life(Actor* actor, float delta) {
    Module* current_module = actor->module;
    do {
        if (current_module->enabled) {
            current_module->life(current_module, delta);
            current_module = current_module->next;
        }
    }
    while(current_module != actor->module);
}
void actor_kill(Actor* actor) {
    Stage* stage = actor->stage;
    if (stage && actor->index_slot >= 0) {
        stage->actor_table[actor->index_slot] = NULL;
    }

    if (actor->module_table)
        free(actor->module_table);
    
    linked_pop_from_list(actor, offsetof(Actor, prev), offsetof(Actor, next));

    actor_simple_kill(actor);
}
void actor_simple_kill(void* actor_pointer) {
    Actor* actor = (Actor*)actor_pointer;
    if (actor->module) {
        linked_kill_list(actor->module, m_simple_kill, offsetof(Module, prev), offsetof(Module, next));
    }

    free(actor);
}

void m_create(Module* module) {
    module->enabled = true;
    module->index_slot = -1;
    module->actor = NULL;
    module->active = m_active;
    module->life = m_life;
    module->inactive = m_inactive;
    module->death = m_death;
    module->prev = module;
    module->next = module;
}
void m_enable(Module* module) {
    if (!module->enabled) {
        module->enabled = true;
        module->life = module->d_life;
        module->active(module);
    }
}
void m_disable(Module* module) {
    if (module->enabled) {
        module->enabled = false;
        module->d_life = module->life;
        module->life = m_life;
        module->inactive(module);
    }
}
void m_kill(Module* module) {
    Actor* actor = module->actor;
    if (actor && module->index_slot >= 0) {
        actor->module_table[module->index_slot] = NULL;
    }
       
    linked_pop_from_list(module, offsetof(Module, prev), offsetof(Module, next));
    m_simple_kill(module);
}
void m_simple_kill(void* module_pointer) {
    Module* module = (Module*)module_pointer;
    module->death(module);
}

void m_active(Module* self) {
}
void m_life(Module* self, float delta) {

}
void m_inactive(Module* self) {
}
void m_death(Module* module) {
    free(module);
}