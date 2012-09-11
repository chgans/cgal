namespace CGAL {

/*!
  \ingroup PkgPolyhedron

  A polyhedral surface `Polyhedron_3` consists of vertices \f$ V\f$, 
  edges \f$ E\f$, facets \f$ F\f$ and an incidence relation on them. Each edge is 
  represented by two halfedges with opposite orientations. 

  \image html halfedge.gif

  Vertices represent points in 3d-space. Edges are straight line segments 
  between two endpoints. Facets are planar polygons without holes 
  defined by the circular sequence of halfedges along their boundary. 
  The polyhedral surface itself can have holes. The halfedges 
  along the boundary of a hole are called <I>border halfedges</I> and 
  have no incident facet. An edge is a <I>border edge</I> if one of 
  its halfedges is a border halfedge. A surface is <I>closed</I> if it 
  contains no border halfedges. A closed surface is a boundary 
  representation for polyhedra in three dimensions. The convention is 
  that the halfedges are oriented counterclockwise around facets as seen 
  from the outside of the polyhedron. An implication is that the 
  halfedges are oriented clockwise around the vertices. The notion of 
  the solid side of a facet as defined by the halfedge orientation 
  extends to polyhedral surfaces with border edges although they do not 
  define a closed object. If normal vectors are considered for the 
  facets, normals point outwards (following the right hand rule). 

  The strict definition can be found in \cite k-ugpdd-99. One 
  implication of this definition is that the polyhedral surface is 
  always an orientable and oriented 2-manifold with border edges, i.e., 
  the neighborhood of each point on the polyhedral surface is either 
  homeomorphic to a disc or to a half disc, except for vertices where 
  many holes and surfaces with boundary can join. Another implication is 
  that the smallest representable surface is a triangle (for polyhedral 
  surfaces with border edges) or a tetrahedron (for polyhedra). Boundary 
  representations of orientable 2-manifolds are closed under Euler 
  operations. They are extended with operations that create or close 
  holes in the surface. 

  Other intersections besides the incidence relation are not allowed, 
  although they are not automatically handled, since self intersections 
  are not easy to check efficiently. `Polyhedron_3` does only 
  maintain the combinatorial integrity of the polyhedral surface (using 
  Euler operations) and does not consider the coordinates of the points 
  or any geometric information. 

  The class `Polyhedron_3` can represent polyhedral surfaces as 
  well as polyhedra. The interface is designed in such a way that it 
  is easy to ignore border edges and work only with polyhedra. 

  The sequence of edges can be ordered in the data structure on request 
  such that the sequence starts with the non-border edges and ends with 
  the border edges. Border edges are then itself ordered such that the 
  halfedge which is incident to the facet comes first and the halfedge 
  incident to the hole comes thereafter. This normalization step counts 
  simultaneously the number of border edges. This number is zero if and 
  only if the surface is a closed polyhedron. Note that this class does 
  not maintain this counter nor the halfedge order during further 
  modifications. There is no automatic caching done for auxiliary 
  information. 

  Parameters 
  -------------- 

  The full template declaration of `Polyhedron_3` states four 
  template parameters: 

\code{.cpp}
template < class PolyhedronTraits_3,
           class PolyhedronItems_3 = CGAL::Polyhedron_items_3,
           template < class T, class I>` 
           class HalfedgeDS = CGAL::HalfedgeDS_default,
           class Alloc = CGAL_ALLOCATOR(int) >
class Polyhedron_3;
\endcode

  The first parameter requires a model of the `PolyhedronTraits_3` 
  concept as argument, for example `CGAL::Polyhedron_traits_3`. The 
  second parameter expects a model of the `PolyhedronItems_3` 
  concept. By default, the class `CGAL::Polyhedron_items_3` is 
  preselected. The third parameter is a class template. A model of the 
  `HalfedgeDS` concept is expected. By default, the class 
  `CGAL::HalfedgeDS_default` is preselected, which is a list based 
  implementation of the halfedge data structure. 
  The fourth parameter `Alloc` requires a standard allocator for 
  \stl container classes. The `rebind` mechanism from `Alloc` 
  will be used to create appropriate allocators internally. A default is 
  provided with the macro `CGAL_ALLOCATOR(int)` from the 
  `<CGAL/memory.h>` header file. 

  \sa `CGAL::Polyhedron_3::Vertex` 
  \sa `CGAL::Polyhedron_3::Halfedge` 
  \sa `CGAL::Polyhedron_3::Facet` 
  \sa `PolyhedronTraits_3` 
  \sa `CGAL::Polyhedron_traits_3<Kernel>` 
  \sa `PolyhedronItems_3` 
  \sa `CGAL::Polyhedron_items_3` 
  \sa `HalfedgeDS` 
  \sa `CGAL::HalfedgeDS_default` 
  \sa `CGAL::Polyhedron_incremental_builder_3<HDS>` 
  \sa `CGAL::Modifier_base`. 

  Example 
  -------------- 

  This example program instantiates a polyhedron using the default 
  traits class and creates a tetrahedron. 

  \cgalexample{polyhedron_prog_simple.cpp} 

*/
template< typename Traits >
class Polyhedron_3 {
public:

  /*!
    \ingroup PkgPolyhedron

    A halfedge is an oriented edge 
    between two vertices. It is always paired with a halfedge pointing in 
    the opposite direction. The `opposite()` member function returns 
    this halfedge of opposite orientation. If a halfedge is incident to a 
    facet the `next()` member function points to the successor 
    halfedge around this facet. For border edges the `next()` member 
    function points to the successor halfedge along the hole. For more 
    than two border edges at a vertex, the next halfedge along a hole is 
    not uniquely defined, but a consistent assignment of the next halfedge 
    will be maintained in the data structure. An invariant is that 
    successive assignments of the form `h = h->next()` cycle 
    counterclockwise around the facet (or hole) and traverse all halfedges 
    incident to this facet (or hole). A similar invariant is that successive 
    assignments of the form `h = h->next()->opposite()` cycle 
    clockwise around the vertex and traverse all halfedges incident to 
    this vertex. Two circulators are provided for these circular orders. 

    \anchor figurePolyOptionalMethods
    \image html poly_optional.gif
    <center><b>The three classes `Vertex`, `Halfedge`, and `Facet` of the polyhedral surface. Member functions with shaded background are mandatory. The others are optionally supported.</b></center>

    The incidences encoded in `opposite()` and `next()` are 
    available for each instantiation of polyhedral surfaces. The other 
    incidences are optionally available as indicated with type tags. The 
    `prev()` member function points to the preceding halfedge around 
    the same facet. It is always available, though it might perform a 
    search around the facet using the `next()` member function to find 
    the previous halfedge if the underlying halfedge data structure does 
    not provide an efficient `prev()` member function for halfedges. 
    Handles to the incident vertex and facet are optionally stored. 

    The circulators are assignable to the `Halfedge_handle`. The 
    circulators are bidirectional if the halfedge provided to the 
    polyhedron with the `Items` template argument provides a member 
    function `prev()`, otherwise they are of the forward category. 

    \sa `CGAL::Polyhedron_3::Vertex` 
    \sa `CGAL::Polyhedron_3::Facet` 
    \sa `CGAL::Polyhedron_3<Traits>`

    Implementation 
    -------------- 

    The member functions `prev()` and `prev_on_vertex()` work in 
    constant time if `Supports_halfedge_prev` \f$ \equiv\f$ 
    `CGAL::Tag_true`. Otherwise both methods search for the previous 
    halfedge around the incident facet. 

  */

  class Halfedge {
  public:

    /// \name Types 
    /// @{

    /*! 
      type of incident vertices. 
    */ 
    typedef Hidden_type Vertex; 

    /*! 
      type of incident facets. 
    */ 
    typedef Hidden_type Facet; 

    /*! 
      handle to vertex. 
    */ 
    typedef Hidden_type Vertex_handle; 

    /*! 
      handle to halfedge. 
    */ 
    typedef Hidden_type Halfedge_handle; 

    /*! 
      handle to facet. 
    */ 
    typedef Hidden_type Facet_handle; 

    /*! 
      circulator of 
      halfedges around a vertex. 
    */ 
    typedef Hidden_type Halfedge_around_vertex_circulator; 

    /*! 
      circulator of 
      halfedges around a facet. 
    */ 
    typedef Hidden_type Halfedge_around_facet_circulator; 

    /*! 

     */ 
    typedef Hidden_type Vertex_const_handle; 

    /*! 

     */ 
    typedef Hidden_type Halfedge_const_handle; 

    /*! 

     */ 
    typedef Hidden_type Facet_const_handle; 

    /*! 

     */ 
    typedef Hidden_type Halfedge_around_vertex_const_circulator; 

    /*! 

     */ 
    typedef Hidden_type Halfedge_around_facet_const_circulator; 

    /*! 
      \f$ \equiv\f$ `CGAL::Tag_true` or 
      `CGAL::Tag_false`. 
    */ 
    typedef Hidden_type Supports_halfedge_prev; 

    /*! 
      \f$ \equiv\f$ `CGAL::Tag_true` or 
      `CGAL::Tag_false`. 
    */ 
    typedef Hidden_type Supports_halfedge_vertex; 

    /*! 
      \f$ \equiv\f$ `CGAL::Tag_true` or 
      `CGAL::Tag_false`. 
    */ 
    typedef Hidden_type Supports_halfedge_face; 

    /// @} 

    /// \name Creation 
    /// @{

    /*! 
      default constructor. 
    */ 
    Halfedge(); 

    /// @} 

    /// \name Operations 
    /// @{

    /*! 

     */ 
    Halfedge_handle opposite(); 

    /*! 
      the opposite halfedge. 
    */ 
    Halfedge_const_handle opposite() const; 

    /*! 

     */ 
    Halfedge_handle next(); 

    /*! 
      the next halfedge around the facet. 
    */ 
    Halfedge_const_handle next() const; 

    /*! 

     */ 
    Halfedge_handle prev(); 

    /*! 
      the previous halfedge around the facet. 
    */ 
    Halfedge_const_handle prev() const; 

    /*! 

     */ 
    Halfedge_handle next_on_vertex(); 

    /*! 

      the next halfedge around the vertex (clockwise). Is equal 
      to `h.next()->opposite()`. 
    */ 
    Halfedge_const_handle next_on_vertex() const; 

    /*! 

     */ 
    Halfedge_handle prev_on_vertex(); 

    /*! 

      the previous halfedge around the vertex (counterclockwise). 
      Is equal to `h.opposite()->prev()`. 
    */ 
    Halfedge_const_handle prev_on_vertex() const; 

    /*! 
      is true if `h` is a border halfedge. 
    */ 
    bool is_border() const; 

    /*! 
      is true if `h` or `h.opposite()` is a border halfedge. 
    */ 
    bool is_border_edge() const; 

    /*! 

     */ 
    Halfedge_around_vertex_circulator vertex_begin(); 

    /*! 
      circulator of halfedges around the vertex (clockwise). 
    */ 
    Halfedge_around_vertex_const_circulator vertex_begin() const; 

    /*! 

     */ 
    Halfedge_around_facet_circulator facet_begin(); 

    /*! 
      circulator of halfedges around the facet (counterclockwise). 
    */ 
    Halfedge_around_facet_const_circulator facet_begin() const; 

    /*! 
      the degree of the 
      incident vertex, i.e., number of edges emanating from this vertex. 
    */ 
    std::size_t vertex_degree() const; 

    /*! 
      returns `true` if the 
      incident vertex has exactly two incident edges. 
    */ 
    bool is_bivalent() const; 

    /*! 
      returns `true` if the 
      incident vertex has exactly three incident edges. 
    */ 
    bool is_trivalent() const; 

    /*! 
      the degree of the 
      incident facet, i.e., number of edges on the boundary of this facet. 
    */ 
    std::size_t facet_degree() const; 

    /*! 
      returns `true` if the 
      incident facet is a triangle. 
    */ 
    bool is_triangle() const; 

    /*! 
      returns `true` if the 
      incident facet is a quadrilateral. 
    */ 
    bool is_quad() const; 

    /// @} 

    /// \name Operations available if Supports_halfedge_vertex is CGAL::Tag_true 
    /// @{

    /*! 

     */ 
    Vertex_handle vertex(); 

    /*! 
      the incident vertex of `h`. 
    */ 
    Vertex_const_handle vertex() const; 

    /// @} 

    /// \name Operations available if Supports_halfedge_facet is CGAL::Tag_true 
    /// @{

    /*! 

     */ 
    Facet_handle facet(); 

    /*! 
      the incident facet of `h`. If `h` is a border halfedge 
      the result is default construction of the handle. 
    */ 
    Facet_const_handle facet() const; 

    /// @}

  }; /* end Halfedge */

  /*!
    \ingroup PkgPolyhedron

    A facet optionally stores a plane equation, and a reference to an 
    incident halfedge that points to the facet. Type tags indicate whether 
    these member functions are supported. 
    Figure \ref figurePolyOptionalMethods 
    depicts the relationship between a halfedge and its incident 
    halfedges, vertices, and facets. The circulator is assignable to the 
    `Halfedge_handle`. The circulator is bidirectional if the 
    halfedge provided to the polyhedron with the `Items` template 
    argument provides a member function `prev()`, otherwise it is 
    of the forward category. 

    Operations available if `Supports_facet_plane` \f$ \equiv\f$ `CGAL::Tag_true` 
    -------------- 

    Operations available if `Supports_facet_halfedge` \f$ \equiv\f$ 
    \sa `CGAL::Polyhedron_3::Vertex` 
    \sa `CGAL::Polyhedron_3::Halfedge` 
    \sa `CGAL::Polyhedron_3<Traits>` 

  */

  class Facet {
  public:

    /// \name Types 
    /// @{

    /*! 
      type of incident halfedges. 
    */ 
    typedef Hidden_type Halfedge; 

    /*! 
      plane equation type stored in facets. 
    */ 
    typedef Hidden_type Plane_3; 

    /*! 
      handle to vertex. 
    */ 
    typedef Hidden_type Vertex_handle; 

    /*! 
      handle to halfedge. 
    */ 
    typedef Hidden_type Halfedge_handle; 

    /*! 
      handle to facet. 
    */ 
    typedef Hidden_type Facet_handle; 

    /*! 
      circulator of 
      halfedges around a facet. 
    */ 
    typedef Hidden_type Halfedge_around_facet_circulator; 

    /*! 

     */ 
    typedef Hidden_type Vertex_const_handle; 

    /*! 

     */ 
    typedef Hidden_type Halfedge_const_handle; 

    /*! 

     */ 
    typedef Hidden_type Facet_const_handle; 

    /*! 

     */ 
    typedef Hidden_type Halfedge_around_facet_const_circulator; 

    /*! 
      \f$ \equiv\f$ `CGAL::Tag_true` or 
      `CGAL::Tag_false`. 
    */ 
    typedef Hidden_type Supports_facet_halfedge; 

    /*! 
      \f$ \equiv\f$ `CGAL::Tag_true` or 
      `CGAL::Tag_false`. 
    */ 
    typedef Hidden_type Supports_facet_plane; 

    /// @} 

    /// \name Creation 
    /// @{

    /*! 
      default constructor. 
    */ 
    Facet(); 

    /// @} 

    /// \name Operations available if Supports_facet_plane is CGAL::Tag_true 
    /// @{

    /*! 

     */ 
    Plane_3& plane(); 

    /*! 
      plane equation. 
    */ 
    const Plane_3& plane() const; 

    /// @} 

    /// \name Operations available if Supports_facet_halfedge is CGAL::Tag_true
    /// @{


    /*! 

     */ 
    Halfedge_handle halfedge(); 

    /*! 

      an incident halfedge that points to `f`. 
    */ 
    Halfedge_const_handle halfedge() const; 

    /*! 

     */ 
    Halfedge_around_facet_circulator facet_begin(); 

    /*! 
      circulator of halfedges around the facet (counterclockwise). 
    */ 
    Halfedge_around_facet_const_circulator facet_begin() const; 

    /*! 
      sets incident halfedge to `h`. 
      \pre `h` is incident, i.e., `h->facet() ==` `f`. 
    */ 
    void set_halfedge( Halfedge_handle h); 

    /*! 
      the degree of the 
      facet, i.e., number of edges on the boundary of this facet. 
    */ 
    std::size_t facet_degree() const; 

    /*! 
      returns `true` if the 
      facet is a triangle. 
    */ 
    bool is_triangle() const; 

    /*! 
      returns `true` if the 
      facet is a quadrilateral. 
    */ 
    bool is_quad() const; 

    /// @}

  }; /* end Facet */

  /*!
    \ingroup PkgPolyhedron

    A vertex optionally stores a point and a reference to an incident 
    halfedge that points to the vertex. Type tags indicate whether these 
    member functions are supported. 
    Figure \ref figurePolyOptionalMethods 
    depicts the relationship between a halfedge and its incident 
    halfedges, vertices, and facets. The circulator is assignable to the 
    `Halfedge_handle`. The circulator is bidirectional if the 
    halfedge provided to the polyhedron with the `Items` template 
    argument provides a member function `prev()`, otherwise it is 
    of the forward category. 

    \sa `CGAL::Polyhedron_3::Halfedge` 
    \sa `CGAL::Polyhedron_3::Facet`
    \sa `CGAL::Polyhedron_3<Traits>` 

  */
  class Vertex {
  public:

    /// \name Types 
    /// @{

    /*! 
      type of incident halfedges. 
    */ 
    typedef Hidden_type Halfedge; 

    /*! 
      type of incident facets. 
    */ 
    typedef Hidden_type Facet; 

    /*! 
      point type stored in vertices. 
    */ 
    typedef Hidden_type Point_3; 

    /*! 
      handle to vertex. 
    */ 
    typedef Hidden_type Vertex_handle; 

    /*! 
      handle to halfedge. 
    */ 
    typedef Hidden_type Halfedge_handle; 

    /*! 
      handle to facet. 
    */ 
    typedef Hidden_type Facet_handle; 

    /*! 
      circulator of 
      halfedges around a vertex. 
    */ 
    typedef Hidden_type Halfedge_around_vertex_circulator; 

    /*! 

     */ 
    typedef Hidden_type Vertex_const_handle; 

    /*! 

     */ 
    typedef Hidden_type Halfedge_const_handle; 

    /*! 

     */ 
    typedef Hidden_type Facet_const_handle; 

    /*! 

     */ 
    typedef Hidden_type Halfedge_around_vertex_const_circulator; 

    /*! 
      \f$ \equiv\f$ `CGAL::Tag_true` or 
      `CGAL::Tag_false`. 
    */ 
    typedef Hidden_type Supports_vertex_halfedge; 

    /*! 
      \f$ \equiv\f$ `CGAL::Tag_true` or 
      `CGAL::Tag_false`. 
    */ 
    typedef Hidden_type Supports_vertex_point; 

    /// @} 

    /// \name Creation 
    /// @{

    /*! 
      default constructor. 
    */ 
    Vertex(); 

    /*! 
      vertex initialized with a point. 
    */ 
    Vertex( const Point& p); 

    /// @} 

    /// \name Operations available if Supports_vertex_point is CGAL::Tag_true 
    /// @{

    /*! 

     */ 
    Point_3& point(); 

    /*! 
      the point. 
    */ 
    const Point_3& point() const; 

    /// @} 

    /// \name Operations available if Supports_vertex_halfedge is  CGAL::Tag_true 
    /// @{

    /*! 

     */ 
    Halfedge_handle halfedge(); 

    /*! 

      an incident halfedge that points to `v`. 
    */ 
    Halfedge_const_handle halfedge() const; 

    /*! 

     */ 
    Halfedge_around_vertex_circulator vertex_begin(); 

    /*! 
      circulator of halfedges around the vertex (clockwise). 
    */ 
    Halfedge_around_vertex_const_circulator vertex_begin() const; 

    /*! 
      sets incident halfedge to `h`. 
      \pre `h` is incident, i.e., `h->vertex() ==` `v`. 
    */ 
    void set_halfedge( Halfedge_handle h); 

    /*! 
      the degree of the 
      vertex, i.e., number of edges emanating from this vertex. 
    */ 
    std::size_t vertex_degree() const; 

    /*! 
      returns `true` if the vertex 
      has exactly two incident edges. 
    */ 
    bool is_bivalent() const; 

    /*! 
      returns `true` if the vertex 
      has exactly three incident edges. 
    */ 
    bool is_trivalent() const; 

    /// @}

  }; /* end Vertex */

  /// \name Types 
  /// @{

  /*! 
    traits class selected for `PolyhedronTraits_3`. 
  */ 
  typedef Hidden_type Traits; 

  /*! 
    items class selected for `PolyhedronItems_3`. 
  */ 
  typedef Hidden_type Items; 

  /*! 
    instantiated halfedge data structure. 
  */ 
  typedef Hidden_type HalfedgeDS; 

  /*! 
    size type of `HalfedgeDS`. 
  */ 
  typedef Hidden_type size_type; 

  /*! 
    difference type of `HalfedgeDS`. 
  */ 
  typedef Hidden_type difference_type; 

  /*! 
    iterator category of `HalfedgeDS` 
    for all iterators. 
  */ 
  typedef Hidden_type iterator_category; 

  /*! 
    circulator category of all circulators; 
    bidirectional category if the `Items::Halfedge` provides a `prev()` 
    member function, otherwise forward category. 
  */ 
  typedef Hidden_type circulator_category; 

  /*! 
    allocator type `Alloc`. 
  */ 
  typedef Hidden_type allocator_type; 

  /// @}


  /*!
    \name Handles, Iterators, and Circulators
    The following handles, iterators, and circulators have appropriate
    non-mutable counterparts, i.e., `const_handle`,
    `const_iterator`, and `const_circulator`. The mutable types are
    assignable to their non-mutable counterparts.  Both circulators are
    assignable to the `Halfedge_iterator`. The iterators are
    assignable to the respective handle types. Wherever the handles appear
    in function parameter lists, the corresponding iterators can be used as
    well. For convenience, the `Edge_iterator` enumerates every other
    halfedge. It is based on the `CGAL::N_step_adaptor` class. For
    convenience, the `Point_iterator` enumerates all points in the polyhedral
    surface in the same order as the `Vertex_iterator`, but with the
    value type `Point`. It is based on the `CGAL::Iterator_project`
    adaptor. Similarly, a `Plane_iterator` is provided.
  */
  /// @{

  /*! 
    point stored in vertices. 
  */ 
  typedef Hidden_type Point_3; 

  /*! 
    plane equation stored in facets (if supported). 
  */ 
  typedef Hidden_type Plane_3; 

  /*! 
    handle to vertex. 
  */ 
  typedef Hidden_type Vertex_handle; 

  /*! 
    handle to halfedge. 
  */ 
  typedef Hidden_type Halfedge_handle; 

  /*! 
    handle to facet. 
  */ 
  typedef Hidden_type Facet_handle; 

  /*! 
    iterator over all vertices. 
  */ 
  typedef Hidden_type Vertex_iterator; 

  /*! 
    iterator over all halfedges. 
  */ 
  typedef Hidden_type Halfedge_iterator; 

  /*! 
    iterator over all facets. 
  */ 
  typedef Hidden_type Facet_iterator; 

  /*! 
    circulator of 
    halfedges around a vertex (cw). 
  */ 
  typedef Hidden_type Halfedge_around_vertex_circulator; 

  /*! 
    circulator of 
    halfedges around a facet (ccw). 
  */ 
  typedef Hidden_type Halfedge_around_facet_circulator; 

  /*! 
    iterator over all edges (every other halfedge). 
  */ 
  typedef Hidden_type Edge_iterator; 

  /*! 
    iterator over all points. 
  */ 
  typedef Hidden_type Point_iterator; 

  /*! 
    iterator over all plane equations. 
  */ 
  typedef Hidden_type Plane_iterator; 

  /// @} 

  /// \name Types for Tagging Optional Features 
  /// The following types are equal to either `CGAL::Tag_true` or
  /// `CGAL::Tag_false`, depending on whether the named feature is
  /// supported or not.
  /// @{

  /*! 
    `Vertex::halfedge()`. 
  */ 
  typedef Hidden_type Supports_vertex_halfedge; 

  /*! 
    `Vertex::point()`. 
  */ 
  typedef Hidden_type Supports_vertex_point; 

  /*! 
    `Halfedge::prev()`. 
  */ 
  typedef Hidden_type Supports_halfedge_prev; 

  /*! 
    `Halfedge::vertex()`. 
  */ 
  typedef Hidden_type Supports_halfedge_vertex; 

  /*! 
    `Halfedge::facet()`. 
  */ 
  typedef Hidden_type Supports_halfedge_facet; 

  /*! 
    `Facet::halfedge()`. 
  */ 
  typedef Hidden_type Supports_facet_halfedge; 

  /*! 
    `Facet::plane()`. 
  */ 
  typedef Hidden_type Supports_facet_plane; 

  /*! 
    supports removal of individual elements. 
  */ 
  typedef Hidden_type Supports_removal; 

  /// @} 

  /// \name Creation 
  /// @{

  /*! 

   */ 
  Polyhedron_3(const Traits& traits = Traits()); 

  /*! 
    a polyhedron `P` with storage reserved 
    for \f$ v\f$ vertices, \f$ h\f$ halfedges, and \f$ f\f$ facets. The 
    reservation sizes are a hint for optimizing storage 
    allocation. 
  */ 
  Polyhedron_3( size_type v, size_type h, size_type f, 
                const Traits& traits = Traits()); 

  /*! 
    reserve storage 
    for \f$ v\f$ vertices, \f$ h\f$ halfedges, and \f$ f\f$ facets. The 
    reservation sizes are a hint for optimizing storage 
    allocation. If the `capacity` is already greater 
    than the requested size nothing happens. If the 
    `capacity` changes all iterators and circulators 
    might invalidate. 
  */ 
  void reserve( size_type v, size_type h, size_type f); 

  /*! 
    a tetrahedron is added to the 
    polyhedral surface. Returns a halfedge of the tetrahedron. 
  */ 
  Halfedge_handle make_tetrahedron(); 

  /*! 

    a tetrahedron is added to the polyhedral surface with its 
    vertices initialized to \f$ p_1, p_2, p_3\f$, and \f$ p_4\f$. Returns that 
    halfedge of the tetrahedron which incident vertex is initialized 
    to \f$ p_1\f$. The incident vertex of the next halfedge is \f$ p_2\f$, 
    and the vertex thereafter is \f$ p_3\f$. 
    The remaining fourth vertex is initialized to \f$ p_4\f$. 
  */ 
  Halfedge_handle make_tetrahedron(const Point& p1, 
                                   const Point& p2, 
                                   const Point& p3, 
                                   const Point& p4); 

  /*! 
    a triangle with border edges is 
    added to the polyhedral surface. Returns a non-border 
    halfedge of the triangle. 
  */ 
  Halfedge_handle make_triangle(); 

  /*! 

    a triangle with border edges is added to the polyhedral surface with its 
    vertices initialized to \f$ p_1, p_2\f$, and \f$ p_3\f$. Returns that 
    non-border halfedge of the triangle which incident vertex is initialized 
    to \f$ p_1\f$. The incident vertex of the next halfedge is \f$ p_2\f$, 
    and the vertex thereafter is \f$ p_3\f$. 
  */ 
  Halfedge_handle make_triangle(const Point& p1, 
                                const Point& p2, 
                                const Point& p3); 

  /// @} 

  /// \name Access Member Functions 
  /// @{

  /*! 
    returns true if `P` is empty. 
  */ 
  bool empty() const; 

  /*! 
    number of vertices. 
  */ 
  size_type size_of_vertices() const; 

  /*! 
    number of halfedges (incl. border halfedges). 
  */ 
  size_type size_of_halfedges() const; 

  /*! 
    number of facets. 
  */ 
  size_type size_of_facets() const; 

  /*! 
    space reserved for vertices. 
  */ 
  size_type capacity_of_vertices() const; 

  /*! 
    space reserved for halfedges. 
  */ 
  size_type capacity_of_halfedges() const; 

  /*! 
    space reserved for facets. 
  */ 
  size_type capacity_of_facets() const; 

  /*! 
    bytes used for the polyhedron. 
  */ 
  size_t bytes() const; 

  /*! 
    bytes reserved for the polyhedron. 
  */ 
  size_t bytes_reserved() const; 

  /*! 
    allocator object. 
  */ 
  allocator_type get_allocator() const; 

  /*! 
    iterator over all vertices. 
  */ 
  Vertex_iterator vertices_begin(); 

  /*! 
    past-the-end iterator. 
  */ 
  Vertex_iterator vertices_end(); 

  /*! 
    iterator over all 
    halfedges. 
  */ 
  Halfedge_iterator halfedges_begin(); 

  /*! 
    past-the-end iterator. 
  */ 
  Halfedge_iterator halfedges_end(); 

  /*! 
    iterator over all facets 
    (excluding holes). 
  */ 
  Facet_iterator facets_begin(); 

  /*! 
    past-the-end iterator. 
  */ 
  Facet_iterator facets_end(); 

  /*! 
    iterator over all edges. 
  */ 
  Edge_iterator edges_begin(); 

  /*! 
    past-the-end iterator. 
  */ 
  Edge_iterator edges_end(); 

  /*! 
    iterator over all points. 
  */ 
  Point_iterator points_begin(); 

  /*! 
    past-the-end iterator. 
  */ 
  Point_iterator points_end(); 

  /*! 
    iterator over all plane equations. 
  */ 
  Plane_iterator planes_begin(); 

  /*! 
    past-the-end iterator. 
  */ 
  Plane_iterator planes_end(); 

  /*! 
    returns the traits class. 
  */ 
  const Traits& traits() const; 

  /// @} 

  /// \name Combinatorial Predicates 
  /// @{

  /*! 
    returns `true` if there are no 
    border edges. 
  */ 
  bool is_closed() const; 

  /*! 
    returns `true` if all 
    vertices have exactly two incident edges. 
  */ 
  bool is_pure_bivalent() const; 

  /*! 
    returns `true` if all 
    vertices have exactly three incident edges. 
  */ 
  bool is_pure_trivalent() const; 

  /*! 
    returns `true` if all 
    facets are triangles. 
  */ 
  bool is_pure_triangle() const; 

  /*! 
    returns `true` if all 
    facets are quadrilaterals. 
  */ 
  bool is_pure_quad() const; 

  /*! 
    `true` 
    iff the connected component denoted by \f$ h\f$ is a triangle. 
  */ 
  bool is_triangle( Halfedge_const_handle h) const; 

  /*! 
    `true` 
    iff the connected component denoted by \f$ h\f$ is a tetrahedron. 
  */ 
  bool is_tetrahedron( Halfedge_const_handle h) const; 

  /// @} 

  /*!
    \name Euler Operators (Combinatorial Modifications) 
    \anchor sectionPolyhedronEuler 

    The following Euler operations modify consistently the combinatorial
    structure of the polyhedral surface. The geometry remains unchanged.

  */
  /// @{

  /*! 
    splits the facet incident to `h` and `g` into two facets 
    with a new diagonal between the two vertices denoted by `h` and 
    `g` respectively. The second (new) facet is a copy of the 
    first facet. Returns `h->next()` after the 
    operation, i.e., the new diagonal. The new face is to the right of the 
    new diagonal, the old face is to the left. The time is 
    proportional to the distance from `h` to `g` around the facet. 
    \pre `h` and `g` are incident to the same facet. `h != g` (no loops). `h->next() != g` and `g->next() != h` (no multi-edges). 

    \image html euler_facet.gif

  */ 
  Halfedge_handle split_facet( Halfedge_handle h, 
                               Halfedge_handle g); 

  /*! 
    joins the two facets incident to \f$ h\f$. The facet incident to 
    `h->opposite()` gets removed. Both facets might be 
    holes. Returns the predecessor of \f$ h\f$ around the facet. The invariant 
    `join_facet( split_facet( h, g))` returns \f$ h\f$ and keeps 
    the polyhedron unchanged. The time is proportional to the size of the 
    facet removed and the time to compute `h->prev()`. 
    \pre The degree of both vertices incident to \f$ h\f$ is at least three (no antennas). 

    Requirement 
    -------------- 

    `Supports_removal` \f$ \equiv\f$ `CGAL::Tag_true`.

    \image html euler_facet.gif

  */ 
  Halfedge_handle join_facet( Halfedge_handle h); 

  /*! 
    splits the vertex incident to `h` and `g` into two vertices, 
    the old vertex remains and a new copy is created, 
    and connects them with a new edge. Let `hnew` be 
    `h->next()->opposite()` after the split, i.e., a halfedge 
    of the new edge. The split regroups the halfedges around the two 
    vertices. The halfedge sequence `hnew`, `g->next()->opposite()`, 
    \f$ \ldots\f$ , `h` remains around the old vertex, while the 
    halfedge sequence `hnew->opposite()`, `h->next()->opposite()` 
    (before the split), \f$ \ldots\f$ , `g` is regrouped around the new 
    vertex. The split returns `hnew`, i.e., the new halfedge incident 
    to the old vertex. The time is proportional to the distance from 
    `h` to `g` around the vertex. 
    \pre `h` and `g` are incident to the same vertex. `h != g` (antennas are not allowed). 

    \image euler_vertex.gif

    Note 
    -------------- 

    A special application of the split is 
    `split_vertex(h,h->next()->opposite())` which is equivalent to an 
    edge split of the halfedge `h->next()` that creates a new 
    vertex on the halfedge `h->next()`. See also `split_edge(h)` 
    below. 
  */ 
  Halfedge_handle split_vertex( Halfedge_handle h, 
                                Halfedge_handle g); 

  /*! 
    joins the two vertices incident to \f$ h\f$. The vertex denoted by 
    `h->opposite()` gets removed. Returns the predecessor of 
    \f$ h\f$ around the vertex, i.e., `h->opposite()->prev()`. 
    The invariant `join_vertex( split_vertex( h, g))` returns 
    \f$ h\f$ and keeps the polyhedron unchanged. 
    The time is proportional to the degree of the vertex removed and 
    the time to compute `h->prev()` and `h->opposite()->prev()`. 
    \pre The size of both facets incident to \f$ h\f$ is at least four (no multi-edges). 

    Requirement 
    -------------- 

    `Supports_removal` \f$ \equiv\f$ `CGAL::Tag_true`. 

    \image euler_vertex.gif
  */ 
  Halfedge_handle join_vertex( Halfedge_handle h); 

  /*! 

    splits the halfedge `h` into two halfedges inserting a new vertex 
    that is a copy of `h->opposite()->vertex()`. Is equivalent to 
    `split_vertex( h->prev(), h->opposite())`. The call of `prev()` 
    can make this method slower than a direct call of `split_vertex()` 
    if the previous halfedge is already known and computing it would be 
    costly when the halfedge data structure does not support the `prev()` 
    member function. Returns the new halfedge `hnew` pointing to the 
    inserted vertex. The new halfedge is followed by the old halfedge, i.e., 
    `hnew->next() == h`. 
  */ 
  Halfedge_handle split_edge( Halfedge_handle h); 

  /*! 
    performs an edge flip. It returns \f$ h\f$ after rotating the edge \f$ h\f$ one 
    vertex in the direction of the face orientation. 
    \pre `h != Halfedge_handle()` and both facets incident to \f$ h\f$ are triangles. 
  */ 
  Halfedge_handle flip_edge( Halfedge_handle h); 

  /*! 
    barycentric triangulation of `h->facet()`. Creates a new vertex, 
    a copy of `h->vertex()`, and connects it to each vertex incident 
    to `h->facet()` splitting `h->facet()` into triangles. 
    `h` remains incident to the original facet, all other triangles 
    are copies of this facet. Returns the halfedge `h->next()` 
    after the operation, i.e., a halfedge pointing to the new vertex. 
    The time is proportional to the size of the facet. 
    \pre `h` is not a border halfedge. 

    \image euler_center.gif

  */ 
  Halfedge_handle create_center_vertex( Halfedge_handle h); 

  /*! 
    reverses `create_center_vertex`. Erases the 
    vertex pointed to by `g` and all incident halfedges thereby 
    merging all incident facets. Only `g->facet()` remains. 
    The neighborhood of `g->vertex()` may not be triangulated, 
    it can have larger facets. Returns the halfedge `g->prev()`. 
    Thus, the invariant `h == erase_center_vertex( create_center_vertex(h))` holds if `h` is not a border halfedge. 
    The time is proportional to the sum of the size of all incident facets. 
    \pre None of the incident facets of `g->vertex()` is a hole. There are at least two distinct facets incident to the facets that are incident to `g->vertex()`. (This prevents the operation from collapsing a volume into two facets glued together with opposite orientations, such as would happen with any vertex of a tetrahedron.) 

    Requirement 
    -------------- 

    `Supports_removal` \f$ \equiv\f$ `CGAL::Tag_true`. 

    \image euler_center.gif
  */ 
  Halfedge_handle erase_center_vertex( Halfedge_handle g); 

  /// @} 

  /// \name Euler Operators Modifying Genus 
  /// @{

  /*! 
    cuts the polyhedron into two parts along the cycle \f$ (h,i,j)\f$ (edge `j` 
    runs on the backside of the three dimensional figure above). 
    Three new vertices (one copy for each vertex in the cycle) and three 
    new halfedges (one copy for each halfedge in the cycle), and two new 
    triangles are created. \f$ h,i,j\f$ will be incident to the first new triangle. 
    The return value will be the halfedge incident to the second new triangle 
    which is the copy of `h-opposite()`. 
    \pre \f$ h,i,j\f$ denote distinct, consecutive vertices of the polyhedron and form a cycle: i.e., `h->vertex() == i->opposite()->vertex()`, \f$ \ldots\f$ , `j->vertex() == h->opposite()->vertex()`. The six facets incident to \f$ h,i,j\f$ are all distinct. 

    \image html euler_loop.gif 
  */ 
  Halfedge_handle split_loop( Halfedge_handle h, 
                              Halfedge_handle i, 
                              Halfedge_handle j); 

  /*! 
    glues the boundary of the two facets denoted by \f$ h\f$ and \f$ g\f$ together 
    and returns \f$ h\f$. Both facets and the vertices along the facet denoted 
    by \f$ g\f$ gets removed. Both facets may be holes. The invariant 
    `join_loop( h, split_loop( h, i, j))` returns \f$ h\f$ and keeps the 
    polyhedron unchanged. 
    \pre The facets denoted by \f$ h\f$ and \f$ g\f$ are different and have equal degree (i.e., number of edges). 

    Requirement 
    -------------- 

    `Supports_removal` \f$ \equiv\f$ 
    `CGAL::Tag_true`. 

    \image html euler_loop.gif

  */ 
  Halfedge_handle join_loop( Halfedge_handle h, Halfedge_handle g); 

  /// @} 

  /// \name Modifying Facets and Holes 

  /// @{

  /*! 
    removes the incident facet of \f$ h\f$ and changes all halfedges incident 
    to the facet into border edges. Returns \f$ h\f$. 
    See `erase_facet(h)` for a more generalized variant. 
    \pre None of the incident halfedges of the facet is a border edge. 

    Requirement 
    -------------- 

    `Supports_removal` \f$ \equiv\f$ 
    `CGAL::Tag_true`. 
  */ 
  Halfedge_handle make_hole( Halfedge_handle h); 

  /*! 

    fills a hole with a newly created facet. Makes all border halfedges 
    of the hole denoted by \f$ h\f$ incident to the new facet. Returns \f$ h\f$. 
    \pre `h.is_border()`. 
  */ 
  Halfedge_handle fill_hole( Halfedge_handle h); 

  /*! 
    creates a new facet within the hole incident to \f$ h\f$ 
    and \f$ g\f$ by connecting the tip of \f$ g\f$ with the tip of \f$ h\f$ 
    with two new halfedges and a new vertex and filling this separated 
    part of the hole with a new facet, such that the new facet is 
    incident to \f$ g\f$. Returns the halfedge of the new edge that is 
    incident to the new facet and the new vertex. 
    \pre `h->is_border()`, `g->is_border()`, `h != g`, and \f$ g\f$ can be reached along the same hole starting with \f$ h\f$. 

    \image html add_facet1.gif
  */ 
  Halfedge_handle add_vertex_and_facet_to_border( 
    Halfedge_handle h, Halfedge_handle g); 

  /*! 
    creates a new facet within the hole incident to \f$ h\f$ and \f$ g\f$ by 
    connecting the vertex denoted by \f$ g\f$ with the vertex denoted by \f$ h\f$ 
    with a new halfedge and filling this separated part of the hole with 
    a new facet, such that the new facet is incident to \f$ g\f$. 
    Returns the halfedge of the new edge that is incident to the new facet. 
    \pre `h->is_border()`, `g->is_border()`, `h != g`, `h->next() != g`, and \f$ g\f$ can be reached along the same hole starting with \f$ h\f$. 

    \image html add_facet2.gif

  */ 
  Halfedge_handle add_facet_to_border( Halfedge_handle h, 
                                       Halfedge_handle g); 

  /// @} 

  /// \name Erasing 
  /// @{

  /*! 
    removes the incident facet of \f$ h\f$ and changes all halfedges incident 
    to the facet into border edges or removes them from the 
    polyhedral surface if they were already border edges. 
    If this creates isolated vertices they get removed as well. 
    See `make_hole(h)` for a more specialized variant. 
    \pre `h->is_border() == false`. 
    Requirement 
    -------------- 

    `Supports_removal` \f$ \equiv\f$ `CGAL::Tag_true`. 

    \image html add_facet1.gif

    \image html add_facet2.gif
  */ 
  void erase_facet( Halfedge_handle h); 

  /*! 
    removes the vertices, halfedges, and facets that belong to the 
    connected component of \f$ h\f$. 

    Requirement 
    -------------- 

    `Supports_removal` \f$ \equiv\f$ 
    `CGAL::Tag_true`. 
  */ 
  void erase_connected_component( Halfedge_handle h); 

  /*! 
    Erases the small connected components and the isolated vertices. 
    Keep `nb_components_to_keep` largest connected components. 
    Returns the number of connected components erased (ignoring isolated vertices). 

    Requirement 
    -------------- 

    supports vertices, halfedges, and removal operation. 
  */ 
  unsigned int keep_largest_connected_components(unsigned int nb_components_to_keep); 

  /*! 
    removes all vertices, halfedges, and facets. 
  */ 
  void clear(); 

  /// @} 

  /*!
    \name Operations with Border Halfedges 

    \advanced Halfedges incident to a hole are called <I>border
    halfedges</I>. An halfedge is a <I>border edge</I> if itself or its
    opposite halfedge are border halfedges. The only requirement to work
    with border halfedges is that the `Halfedge` class provides a member
    function `is_border()` returning a `bool`. Usually, the halfedge data
    structure supports facets and a `NULL` facet pointer will indicate a
    border halfedge, but this is not the only possibility. The
    `is_border()` predicate divides the edges into two classes, the border
    edges and the non-border edges. The following normalization
    reorganizes the sequential storage of the edges such that the
    non-border edges precede the border edges, and that for each border
    edge the latter one of the two halfedges is a border halfedge (the
    first one is a non-border halfedge in conformance with the polyhedral
    surface definition). The normalization stores the number of border
    halfedges and the halfedge iterator the border edges start at within
    the data structure. Halfedge insertion or removal and changing the
    border status of a halfedge invalidate these values. They are not
    automatically updated.
  */
  /// @{

  /*! 
    sorts halfedges such that the non-border edges precede the 
    border edges. For each border edge the halfedge iterator will 
    reference the halfedge incident to the facet right before the 
    halfedge incident to the hole. 
  */ 
  void normalize_border(); 

  /*! 
    number of border halfedges. 
    \pre last `normalize_border()` call still valid, see above. 
  */ 
  size_type size_of_border_halfedges() const; 

  /*! 
    number of border edges. Since each border edge of a polyhedral 
    surface has exactly one border halfedge, 
    this number is equal to `size_of_border_halfedges()`. 
    \pre last `normalize_border()` call still valid, see above. 
  */ 
  size_type size_of_border_edges() const; 

  /*! 
    halfedge iterator starting with the border edges. The range 
    [`halfedges_begin(), border_halfedges_begin()`) denotes 
    all non-border halfedges. The range 
    [`border_halfedges_begin(), halfedges_end()`) denotes all 
    border edges. 
    \pre last `normalize_border()` call still valid, see above. 
  */ 
  Halfedge_iterator border_halfedges_begin(); 

  /*! 
    edge iterator starting with the border edges. The range 
    [`edges_begin(), border_edges_begin()`) denotes 
    all non-border edges. The range 
    [`border_edges_begin(), edges_end()`) denotes all 
    border edges. 
    \pre last `normalize_border()` call still valid, see above. 
  */ 
  Edge_iterator border_edges_begin(); 

  /// @} 

  /// \name Miscellaneous 
  /// @{

  /*! 
    reverses facet orientations (incl. plane equations if supported). 
  */ 
  void inside_out(); 

  /*! 
    returns `true` if the polyhedral surface is combinatorially 
    consistent. If `verbose` is `true`, statistics are 
    printed to `cerr`. For `level == 1` the normalization of the 
    border edges is checked too. This method checks in particular level 3 of 
    `CGAL::Halfedge_data_structure_decorator::is_valid` from 
    page \ref pageHalfedgeDSconstDecoratorRef and that each facet is at least 
    a triangle and that the two incident facets of a non-border edge are 
    distinct. 
  */ 
  bool is_valid( bool verbose = false, int level = 0) const; 

  /*! 
    returns `true` if the border halfedges are in normalized 
    representation, which is when enumerating all halfedges with the 
    iterator: The non-border edges precede the border edges and for 
    border edges, the second halfedge is the border halfedge. The halfedge 
    iterator `border_halfedges_begin()` denotes the first border 
    edge. If `verbose` is `true`, statistics are 
    printed to `cerr`. 

  */ 
  bool normalized_border_is_valid( bool verbose = false) const; 

  /*!  
    \advanced calls the `operator()` of the modifier \f$ m\f$. See
    `CGAL::Modifier_base` for a description of modifier design and its
    usage.
    \pre The polyhedral surface must be valid when the modifier returns from execution. 
  */ 
  void delegate( CGAL::Modifier_base<HDS>& m); 

  /// @}

}; /* end Polyhedron_3 */
} /* end namespace CGAL */
