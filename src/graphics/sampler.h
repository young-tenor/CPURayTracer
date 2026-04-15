#pragma once

class Sampler {
public:
    enum class Wrap { repeat, clamp };
    enum class Filter { nearest, linear };

    Wrap wrap;
    Filter filter;

    Sampler(Wrap wrap, Filter filter) : wrap(wrap), filter(filter) {}
};
