#include "graham.h"

using namespace std;

Graham::Graham(vector<Point> points): _points(points){
  int n = _points.size();
    
  // Trouver le point avec la plus petite coordonnée y (et la plus petite coordonnée x en cas d'égalité)
  int minY = _points[0].y, minIndex = 0;
  for (int i = 1; i < n; i++) {
      int y = _points[i].y;
      if ((y < minY) || (y == minY && _points[i].x < _points[minIndex].x)) {
          minY = _points[i].y;
          minIndex = i;
      }
  }

  // Mettre le point pivot à la première position
  swap(_points[0], _points[minIndex]);
  _pivot_point = _points[0];
}

// Fonction pour trouver l'orientation des triplettes de points (p, q, r)
// Retourne :
// -1 si les points sont en virage à droite
//  0 si les points sont colinéaires
//  1 si les points sont en virage à gauche
int orientation(const Point& p, const Point& q, const Point& r) {
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0) return 0;  // colinéaires
    return (val > 0) ? -1 : 1;  // virage à droite ou à gauche
}

int distSq(const Point& p1, const Point& p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

template <typename T>
T nextToTop(stack<T>& s) {
    T topElement = s.top();
    s.pop();
    T nextToTopElement = s.top();
    s.push(topElement);
    return nextToTopElement;
}

// Fonction pour trouver l'enveloppe convexe
void Graham::convexHull() {
  int n = _points.size();

  // Trier les points par angle polaire par rapport au point pivot
  sort(_points.begin() + 1, _points.end(),
    // Comparateur pour trier les points en fonction de l'angle polaire
    [this](const Point& p1, const Point& p2) {
    int o = orientation(_points[0], p1, p2);
    if (o == 0) {
      // En cas de colinéarité, sélectionne le point le plus proche du point de référence (_pivot_point)
      return (distSq(_points[0], p2) >= distSq(_points[0], p1));
    }
    return (o == -1);
  });


  // Initialiser la pile pour stocker l'enveloppe convexe
  stack<Point> hull;
  hull.push(_points[0]);
  hull.push(_points[1]);

  // Parcourir les points triés
  for (int i = 2; i < n; i++) {
    while (orientation(nextToTop(hull), hull.top(), _points[i]) != -1) {
        hull.pop();
    }
    hull.push(_points[i]);
  }

  // Transférer les points de la pile à un vecteur résultant
  while (!hull.empty()) {
    _convex_hull_points.push_back(hull.top());
    hull.pop();
  }

  // Inverser l'ordre pour avoir l'enveloppe convexe dans le sens horaire
  reverse(_convex_hull_points.begin(), _convex_hull_points.end());
}

vector<Point> Graham::getConvexHullPoints() const{
  return _convex_hull_points;
}