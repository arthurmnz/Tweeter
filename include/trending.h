#ifndef TRENDING_H
#define TRENDING_H

typedef struct Topic {
    char name[50];
    unsigned posts_count;
} topic;

typedef struct TrendingTopics {
    topic *topics;
    unsigned size;
    unsigned capacity;
} trending_topics;



void init_trending_topics(trending_topics *, unsigned );

int add_new_topic(trending_topics *, char *);

int increment_topic(trending_topics *, char *);

int is_hashtag_end(char);

int get_trending_topics(trending_topics *);

int show_trending_topics(unsigned );

#endif //TRENDING_H