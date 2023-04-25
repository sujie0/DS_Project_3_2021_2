#ifndef RESULT_H
#define RESULT_H

enum Result
{
    Success = 0,

    InvalidOptionNumber = 001,
    StringDoesNotMatch, //During rabinkarp, An error that occurs when the strings do not match

    FaildtoUpdatePath = 005,

    CommandFileNotExist = 100,
    LoadFileNotExist,

    InvalidVertexKey=200,
    GraphNotExist=202,
    InvalidAlgorithm,
    NegativeCycleDetected,

    NonDefinedCommand = 300,
};

#endif
