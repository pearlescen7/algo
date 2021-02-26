- init:
    run: rm -f hw3
    blocker: true

- build:
    run: g++ -std=c++11 -Wall -Werror main.cpp -o hw3                                        # timeout: 8
    blocker: true


- case1:
    run: ./hw3 Network.txt Messages.txt
    points: 10
    script:
        - expect: "[ \r\n]*Traversing:0 1[ \r\n]*"                                     # timeout: 2 
        - expect: "[ \r\n]*Message:message1 To:0 1 mh_1[ \r\n]*"                       # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case2:
    run: ./hw3 Network.txt Messages.txt
    points: 10
    script:
        - expect: "[ \r\n]*Traversing:0 1 2[ \r\n]*"                                   # timeout: 2 
        - expect: "[ \r\n]*Message:message2 To:0 2 mh_2[ \r\n]*"                       # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case3:
    run: ./hw3 Network.txt Messages.txt
    points: 10
    script:
        - expect: "[ \r\n]*Traversing:0 1 2 3[ \r\n]*"                                 # timeout: 2 
        - expect: "[ \r\n]*Message:message3 To:0 2 3 mh_3[ \r\n]*"                     # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case4:
    run: ./hw3 Network.txt Messages.txt
    points: 10
    script:
        - expect: "[ \r\n]*Traversing:0 1 2 3 4[ \r\n]*"                               # timeout: 2 
        - expect: "[ \r\n]*Message:message4 To:0 2 3 4 mh_4[ \r\n]*"                   # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case5:
    run: ./hw3 Network.txt Messages.txt
    points: 10
    script:
        - expect: "[ \r\n]*Traversing:0 1 2 3 4 5[ \r\n]*"                             # timeout: 2 
        - expect: "[ \r\n]*Message:message5 To:0 2 3 4 5 mh_5[ \r\n]*"                 # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case6:
    run: ./hw3 Network.txt Messages.txt
    points: 5
    script:
        - expect: "[ \r\n]*Traversing:0 1 2 3 4 5 6[ \r\n]*"                           # timeout: 2 
        - expect: "[ \r\n]*Message:message6 To:0 2 3 4 5 6 mh_6[ \r\n]*"               # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case7:
    run: ./hw3 Network.txt Messages.txt
    points: 5
    script:
        - expect: "[ \r\n]*Traversing:0 1 2 3 4 5 6 7[ \r\n]*"                         # timeout: 2 
        - expect: "[ \r\n]*Message:message7 To:0 2 7 mh_7[ \r\n]*"                     # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case8:
    run: ./hw3 Network.txt Messages.txt
    points: 5
    script:
        - expect: "[ \r\n]*Traversing:0 1 2 3 4 5 6 7 8[ \r\n]*"                       # timeout: 2 
        - expect: "[ \r\n]*Message:message8 To:0 2 7 8 mh_8[ \r\n]*"                   # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case9:
    run: ./hw3 Network.txt Messages.txt
    points: 5
    script:
        - expect: "[ \r\n]*Traversing:0 1 2 3 4 5 6 7 8 9[ \r\n]*"                     # timeout: 2 
        - expect: "[ \r\n]*Message:message9 To:0 2 7 9 mh_9[ \r\n]*"                   # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case10:
    run: ./hw3 Network.txt Messages.txt
    points: 5
    script:
        - expect: "[ \r\n]*Traversing:0 1 2 3 4 5 6 7 8 9 10[ \r\n]*"                  # timeout: 2 
        - expect: "[ \r\n]*Message:message10 To:0 2 7 10 mh_10[ \r\n]*"                # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case11:
    run: ./hw3 Network.txt Messages.txt
    points: 5
    script:
        - expect: "[ \r\n]*Traversing:0 1 2 3 4 5 6 7 8 9 10 11[ \r\n]*"               # timeout: 2 
        - expect: "[ \r\n]*Message:message11 To:0 11 mh_11[ \r\n]*"                    # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case12:
    run: ./hw3 Network.txt Messages.txt
    points: 5
    script:
        - expect: "[ \r\n]*Traversing:0 1 2 3 4 5 6 7 8 9 10 11[ \r\n]*"               # timeout: 2 
        - expect: "[ \r\n]*Message:message12 To:0 11 mh_12[ \r\n]*"                    # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case13:
    run: ./hw3 Network.txt Messages.txt
    points: 5
    script:
        - expect: "[ \r\n]*Traversing:0 1 2 3 4 5 6 7 8 9 10 11[ \r\n]*"               # timeout: 2 
        - expect: "[ \r\n]*Message:message13 To:0 11 mh_13[ \r\n]*"                    # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case14:
    run: ./hw3 Network.txt Messages.txt
    points: 5
    script:
        - expect: "[ \r\n]*Traversing:0 1 2 3 4 5 6 7 8 9 10 11[ \r\n]*"               # timeout: 2 
        - expect: "[ \r\n]*Message:message14 To:0 11 mh_14[ \r\n]*"                    # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0

- case15:
    run: ./hw3 Network.txt Messages.txt
    points: 5
    script:
        - expect: "[ \r\n]*Traversing:0 1 2 3 4 5 6 7 8 9 10 11[ \r\n]*"               # timeout: 2 
        - expect: "[ \r\n]*Message:message15 To:0 11 mh_15[ \r\n]*"                    # timeout: 2
        - expect: _EOF_ # timeout: 2
    return: 0
