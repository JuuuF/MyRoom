
/** ===========================================================================
  pixels.h

  This file contains the parameters for the lamp. Yes, I actually put these
  values in manually.

  The init_lamp() function call is mandatory to be called if the use of edges
  and vertices is desires.

  This file provides:
    Pixel lamp[NUM_LEDs]            lamp pixel coordinates
    uint16_t lamp_x                 max x coordinate of lamp
    uint16_t lamp_y                 max y coordinate of lamp
    Vertex V[]                      physical lamp vertices
    Edge E[]                        physical lamp edges
*/

#pragma once

struct Pixel {
  uint16_t x, y;
  Pixel(uint16_t x_, uint16_t y_) : x(x_), y(y_) {}
};

Pixel lamp[392] {
  // 0
  Pixel(0, 730), Pixel(0, 698), Pixel(0, 662), Pixel(0, 628), Pixel(0, 596), Pixel(0, 562), Pixel(0, 528), Pixel(0, 492), Pixel(0, 460), Pixel(0, 426), Pixel(0, 392), Pixel(0, 358), Pixel(0, 324), Pixel(0, 290), Pixel(0, 258), Pixel(0, 222), Pixel(0, 190), Pixel(0, 156), Pixel(0, 122), Pixel(0, 88), Pixel(0, 54), Pixel(0, 20),
  // 1
  Pixel(11, 17), Pixel(30, 41), Pixel(51, 68), Pixel(73, 95), Pixel(94, 121), Pixel(115, 148), Pixel(136, 175), Pixel(157, 201), Pixel(178, 228), Pixel(199, 255), Pixel(219, 281), Pixel(240, 308), Pixel(260, 334), Pixel(283, 363),
  // 2
  Pixel(270, 390), Pixel(250, 417), Pixel(231, 422), Pixel(208, 471), Pixel(188, 497), Pixel(167, 524), Pixel(146, 551), Pixel(125, 578), Pixel(104, 605), Pixel(83, 632), Pixel(62, 659), Pixel(41, 686), Pixel(20, 713),
  // 3
  Pixel(26, 740), Pixel(60, 740), Pixel(94, 740), Pixel(128, 740), Pixel(163, 740), Pixel(197, 740), Pixel(231, 740), Pixel(264, 740), Pixel(299, 740), Pixel(333, 740), Pixel(366, 740), Pixel(400, 740), Pixel(433, 740),
  // 4
  Pixel(441, 709), Pixel(427, 678), Pixel(413, 648), Pixel(399, 617), Pixel(386, 587), Pixel(372, 557), Pixel(358, 526), Pixel(345, 496), Pixel(330, 464), Pixel(317, 435), Pixel(302, 404),
  // 5
  Pixel(310, 379), Pixel(340, 392), Pixel(371, 405), Pixel(402, 418), Pixel(432, 431), Pixel(464, 445), Pixel(494, 457), Pixel(524, 470), Pixel(555, 483), Pixel(586, 496), Pixel(617, 509), Pixel(648, 523), Pixel(679, 535), Pixel(708, 548), Pixel(740, 561), Pixel(771, 574), Pixel(801, 586), Pixel(832, 599), Pixel(836, 611),
  // 6
  Pixel(873, 631), Pixel(840, 640), Pixel(808, 649), Pixel(777, 658), Pixel(743, 667), Pixel(711, 676), Pixel(679, 685), Pixel(647, 694), Pixel(614, 703), Pixel(582, 712), Pixel(549, 720), Pixel(518, 729),
  // 7
  Pixel(474, 740), Pixel(512, 740), Pixel(541, 740), Pixel(574, 740), Pixel(609, 740), Pixel(642, 740), Pixel(675, 740), Pixel(708, 740), Pixel(742, 740), Pixel(775, 740), Pixel(808, 740), Pixel(842, 740), Pixel(876, 740), Pixel(909, 740), Pixel(942, 740), Pixel(975, 740), Pixel(1007, 740), Pixel(1041, 740), Pixel(1075, 740), Pixel(1108, 740), Pixel(1141, 740), Pixel(1175, 740), Pixel(1209, 740), Pixel(1242, 740), Pixel(1275, 740),
  // 8
  Pixel(1279, 728), Pixel(1247, 718), Pixel(1214, 708), Pixel(1182, 700), Pixel(1150, 690), Pixel(1118, 682), Pixel(1085, 673), Pixel(1053, 664), Pixel(1021, 655), Pixel(991, 647), Pixel(958, 638), Pixel(926, 629),
  // 9
  Pixel(926, 614), Pixel(958, 605), Pixel(990, 597), Pixel(1023, 588), Pixel(1055, 580), Pixel(1088, 572), Pixel(1121, 564), Pixel(1153, 556), Pixel(1186, 547), Pixel(1218, 539), Pixel(1250, 530), Pixel(1282, 522), Pixel(1315, 513), Pixel(1347, 505), Pixel(1380, 496), Pixel(1412, 488), Pixel(1443, 480), Pixel(1475, 471), Pixel(1508, 462), Pixel(1540, 453), Pixel(1573, 445), Pixel(1605, 436), Pixel(1637, 427),
  // 10
  Pixel(1632, 445), Pixel(1609, 479), Pixel(1585, 504), Pixel(1561, 528), Pixel(1538, 552), Pixel(1515, 576), Pixel(1491, 601), Pixel(1468, 624), Pixel(1444, 649), Pixel(1422, 673), Pixel(1398, 696), Pixel(1374, 723),
  // 11
  Pixel(1326, 740), Pixel(1360, 740), Pixel(1394, 740), Pixel(1428, 740), Pixel(1462, 740), Pixel(1496, 740), Pixel(1530, 740), Pixel(1564, 740), Pixel(1598, 740), Pixel(1632, 740), Pixel(1664, 740), Pixel(1698, 740), Pixel(1731, 740), Pixel(1765, 740),
  // 12
  Pixel(1764, 701), Pixel(1754, 667), Pixel(1743, 635), Pixel(1733, 603), Pixel(1722, 570), Pixel(1711, 539), Pixel(1700, 506), Pixel(1689, 475), Pixel(1677, 441),
  // 13
  Pixel(1670, 405), Pixel(1679, 374), Pixel(1688, 343), Pixel(1697, 311), Pixel(1706, 278), Pixel(1716, 245), Pixel(1725, 214), Pixel(1735, 181), Pixel(1744, 149), Pixel(1753, 116), Pixel(1762, 84), Pixel(1772, 53),
  // 14
  Pixel(1780, 19), Pixel(1780, 53), Pixel(1780, 88), Pixel(1780, 121), Pixel(1780, 155), Pixel(1780, 188), Pixel(1780, 221), Pixel(1780, 255), Pixel(1780, 289), Pixel(1780, 322), Pixel(1780, 355), Pixel(1780, 390), Pixel(1780, 424), Pixel(1780, 458), Pixel(1780, 492), Pixel(1780, 527), Pixel(1780, 559), Pixel(1780, 593), Pixel(1780, 628), Pixel(1780, 662), Pixel(1780, 695), Pixel(1780, 727),
  // 15
  Pixel(1761, 0), Pixel(1727, 0), Pixel(1694, 0), Pixel(1661, 0), Pixel(1627, 0), Pixel(1594, 0), Pixel(1561, 0), Pixel(1527, 0), Pixel(1494, 0), Pixel(1461, 0), Pixel(1428, 0),
  // 16
  Pixel(1430, 25), Pixel(1446, 53), Pixel(1463, 82), Pixel(1479, 110), Pixel(1496, 138), Pixel(1513, 168), Pixel(1530, 196), Pixel(1547, 225), Pixel(1565, 254), Pixel(1581, 282), Pixel(1599, 312), Pixel(1616, 341), Pixel(1634, 370), Pixel(1650, 397),
  // 17
  Pixel(1620, 409), Pixel(1587, 401), Pixel(1555, 393), Pixel(1523, 385), Pixel(1491, 377), Pixel(1458, 369), Pixel(1426, 361), Pixel(1394, 353), Pixel(1361, 344), Pixel(1330, 336), Pixel(1297, 328), Pixel(1265, 320), Pixel(1234, 312), Pixel(1202, 304), Pixel(1169, 296), Pixel(1137, 288), Pixel(1104, 279), Pixel(1073, 271),
  // 18
  Pixel(1063, 253), Pixel(1088, 234), Pixel(1115, 215), Pixel(1143, 195), Pixel(1170, 176), Pixel(1196, 157), Pixel(1225, 137), Pixel(1252, 117), Pixel(1279, 97), Pixel(1306, 78), Pixel(1333, 58), Pixel(1360, 38), Pixel(1388, 17),
  // 19
  Pixel(1389, 0), Pixel(1356, 0), Pixel(1322, 0), Pixel(1289, 0), Pixel(1256, 0), Pixel(1223, 0), Pixel(1189, 0), Pixel(1154, 0), Pixel(1121, 0), Pixel(1087, 0), Pixel(1054, 0), Pixel(1022, 0), Pixel(987, 0), Pixel(954, 0), Pixel(921, 0), Pixel(888, 0), Pixel(855, 0), Pixel(822, 0), Pixel(787, 0), Pixel(755, 0), Pixel(722, 0), Pixel(689, 0), Pixel(653, 0), Pixel(620, 0), Pixel(586, 0), Pixel(553, 0), Pixel(515, 0),
  // 20
  Pixel(526, 19), Pixel(555, 33), Pixel(587, 49), Pixel(617, 63), Pixel(647, 77), Pixel(678, 92), Pixel(708, 106), Pixel(738, 120), Pixel(768, 135), Pixel(798, 150), Pixel(828, 164), Pixel(858, 179), Pixel(888, 193), Pixel(918, 207), Pixel(947, 222), Pixel(978, 236), Pixel(1009, 250),
  // 21
  Pixel(1036, 298), Pixel(1023, 328), Pixel(1010, 360), Pixel(996, 391), Pixel(982, 420), Pixel(968, 452), Pixel(954, 481), Pixel(941, 511), Pixel(927, 542), Pixel(913, 572), Pixel(900, 603),
  // 22
  Pixel(1017, 277), Pixel(984, 280), Pixel(952, 284), Pixel(919, 288), Pixel(886, 292), Pixel(853, 297), Pixel(819, 302), Pixel(786, 305), Pixel(752, 310), Pixel(719, 314), Pixel(686, 319), Pixel(654, 323), Pixel(619, 328), Pixel(586, 332), Pixel(552, 336), Pixel(520, 341), Pixel(485, 345), Pixel(452, 349), Pixel(419, 354), Pixel(385, 359), Pixel(354, 363), Pixel(318, 367),
  // 23
  Pixel(301, 338), Pixel(316, 306), Pixel(330, 276), Pixel(334, 246), Pixel(359, 215), Pixel(373, 185), Pixel(387, 154), Pixel(401, 124), Pixel(416, 94), Pixel(430, 63), Pixel(444, 32),
  // 24
  Pixel(482, 0), Pixel(449, 0), Pixel(416, 0), Pixel(383, 0), Pixel(349, 0), Pixel(316, 0), Pixel(283, 0), Pixel(248, 0), Pixel(215, 0), Pixel(181, 0), Pixel(146, 0), Pixel(112, 0), Pixel(78, 0), Pixel(42, 0), Pixel(6, 0)
};

uint16_t lamp_x = 1780;
uint16_t lamp_y = 740;


/** -----------------------------------------------------------------
  Edge

  Description of an edge inside the lamp. An edge consists of a start index and
  an end index, start <= end.
  Both points are included in the edge.
*/
class Edge
{
  private:
    uint16_t _start, _end;           // start and end pixel indices

  public:
    Edge(uint16_t start, uint16_t end) :
      _start(min(start, end)),
      _end(  max(start, end))
    {}

    uint16_t get_start() { return _start; }

    uint16_t get_end() { return _end; }
};



/** -----------------------------------------------------------------
  Vertex

  Description of a vertex inside the lamp.
  A vertex consists of end points of multiple edges.
*/
class Vertex
{
  private:
    vector<Edge> _edges;            // adjacent edges
    vector<uint16_t> _indices;      // according indices
    uint8_t _size;                  // amount of adjacent edges

  public:
    Vertex() : _size(0) {}

    /**
      addEdge

      Add a new edge point to the vertex.

      Parameters:
        Edge e                      Edge object
        bool start                  true: add edge start, else end
    */
    void add(Edge e, bool start)
    {
      _edges.push_back(e);
      _indices.push_back(start ? e.get_start() : e.get_end());
      _size++;
    }

    /* getter */
    vector<Edge> get_edges() { return _edges; }
    vector<uint16_t> get_indices() { return _indices; }
    uint8_t get_size() { return _size; }
};

Edge E[] {
  Edge(0,21),
  Edge(22,35),
  Edge(36,48),
  Edge(49,61),
  Edge(62,72),
  Edge(73,91),
  Edge(92,103),
  Edge(104,128),
  Edge(129,140),
  Edge(141,163),
  Edge(164,175),
  Edge(176,189),
  Edge(190,198),
  Edge(199,210),
  Edge(211,232),
  Edge(233,243),
  Edge(244,257),
  Edge(258,275),
  Edge(276,288),
  Edge(289,315),
  Edge(316,332),
  Edge(333,343),
  Edge(344,365),
  Edge(366,376),
  Edge(377,391)
};

Vertex V[12];

#define EDGE_START true
#define EDGE_END   false

/**
  init_lamp

  Initialize the lamp parameters. This only includes vertex initialization as
  they are weird to work with. My current C++ knowledge does not allow me to
  do it elegantly else.
*/
void init_lamp() {
  V[0] = Vertex();
  V[0].add(E[0], EDGE_END);
  V[0].add(E[1], EDGE_START);
  V[0].add(E[24], EDGE_END);

  V[1] = Vertex();
  V[1].add(E[19], EDGE_END);
  V[1].add(E[20], EDGE_START);
  V[1].add(E[23], EDGE_END);
  V[1].add(E[24], EDGE_START);

  V[2] = Vertex();
  V[2].add(E[15], EDGE_END);
  V[2].add(E[16], EDGE_START);
  V[2].add(E[18], EDGE_END);
  V[2].add(E[19], EDGE_START);

  V[3] = Vertex();
  V[3].add(E[13], EDGE_END);
  V[3].add(E[14], EDGE_START);
  V[3].add(E[15], EDGE_START);

  V[4] = Vertex();
  V[4].add(E[1], EDGE_END);
  V[4].add(E[2], EDGE_START);
  V[4].add(E[4], EDGE_END);
  V[4].add(E[5], EDGE_START);
  V[4].add(E[22], EDGE_END);
  V[4].add(E[23], EDGE_START);

  V[5] = Vertex();
  V[5].add(E[17], EDGE_END);
  V[5].add(E[18], EDGE_START);
  V[5].add(E[20], EDGE_END);
  V[5].add(E[21], EDGE_START);
  V[5].add(E[22], EDGE_START);

  V[6] = Vertex();
  V[6].add(E[9], EDGE_END);
  V[6].add(E[10], EDGE_START);
  V[6].add(E[12], EDGE_END);
  V[6].add(E[13], EDGE_START);
  V[6].add(E[16], EDGE_END);
  V[6].add(E[17], EDGE_START);

  V[7] = Vertex();
  V[7].add(E[5], EDGE_END);
  V[7].add(E[6], EDGE_START);
  V[7].add(E[8], EDGE_END);
  V[7].add(E[9], EDGE_START);
  V[7].add(E[21], EDGE_END);

  V[8] = Vertex();
  V[8].add(E[0], EDGE_START);
  V[8].add(E[2], EDGE_END);
  V[8].add(E[3], EDGE_START);

  V[9] = Vertex();
  V[9].add(E[3], EDGE_END);
  V[9].add(E[4], EDGE_START);
  V[9].add(E[6], EDGE_END);
  V[9].add(E[7], EDGE_START);

  V[10] = Vertex();
  V[10].add(E[7], EDGE_END);
  V[10].add(E[8], EDGE_START);
  V[10].add(E[10], EDGE_END);
  V[10].add(E[11], EDGE_START);

  V[11] = Vertex();
  V[11].add(E[11], EDGE_END);
  V[11].add(E[12], EDGE_START);
  V[11].add(E[14], EDGE_END);
}