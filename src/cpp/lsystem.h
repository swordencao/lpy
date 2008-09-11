/* ---------------------------------------------------------------------------
#
#       L-Py: L-systems in Python
#
#       Copyright 2003-2008 UMR Cirad/Inria/Inra Dap - Virtual Plant Team
#
#       File author(s): F. Boudon (frederic.boudon@cirad.fr)
#
# ---------------------------------------------------------------------------
#
#                      GNU General Public Licence
#
#       This program is free software; you can redistribute it and/or
#       modify it under the terms of the GNU General Public License as
#       published by the Free Software Foundation; either version 2 of
#       the License, or (at your option) any later version.
#
#       This program is distributed in the hope that it will be useful,
#       but WITHOUT ANY WARRANTY; without even the implied warranty of
#       MERCHANTABILITY or FITNESS For A PARTICULAR PURPOSE. See the
#       GNU General Public License for more details.
#
#       You should have received a copy of the GNU General Public
#       License along with this program; see the file COPYING. If not,
#       write to the Free Software Foundation, Inc., 59
#       Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
# ---------------------------------------------------------------------------
*/

#ifndef __PGL_LSYSTEM_H__
#define __PGL_LSYSTEM_H__

#include "lsysrule.h"
#include "lsyscontext.h"
#include "stringmatching.h"
#include <QtCore/QMutex>

LPY_BEGIN_NAMESPACE

#define MULTI_THREADED_LSYSTEM

/*---------------------------------------------------------------------------*/

typedef std::vector<LsysRule> RuleSet;
typedef std::vector<const LsysRule *> RulePtrSet;

/*---------------------------------------------------------------------------*/

class LPY_API Lsystem {
public:
  /** The different type of rules */
  enum eRuleType {
      eProduction,
      eDecomposition,
      eHomomorphism
  };

  /** Constructor and Destructor */
  Lsystem();
  Lsystem(const std::string& filename);
  ~Lsystem();

  /** clear */
  void clear();

  /** rules nb query */
  bool empty() const;
  size_t nbProductionRules( size_t group ) const;
  size_t nbDecompositionRules( size_t group ) const ;
  size_t nbHomomorphismRules( size_t group ) const;
  size_t nbTotalRules( ) const;
  size_t nbGroups( ) const;

  /** print */
  std::string str() const ;
  std::string code() const;

  /** compile */
  bool compiled();
  void compile();

  /** Filename */
  void setFilename( const std::string& filename );
  std::string getFilename( ) const ;
  std::string getShortFilename( ) const ;

  /** build from file/string */
  void read( const std::string& filename );
  void set( const std::string& rules , std::string * pycode = NULL);

  /** set rules */
  void addRule( const LsysRule& rule, int type, size_t group );
  void addRule( const std::string& rule, int type, size_t group );
  void addProdRule( const std::string& rule, size_t group );
  void addDecRule( const std::string& rule, size_t group );
  void addHomRule( const std::string& rule, size_t group );

  /** get rules */
  inline const LsysRule& prodRule( size_t i, size_t group) const
  { return __rules[group].production[i]; }
  inline const LsysRule& decRule( size_t i, size_t group) const
  { return __rules[group].decomposition[i]; }
  inline const LsysRule& homRule( size_t i, size_t group) const
  { return __rules[group].homomorphism[i]; }

  /** Axiom */
  void setAxiom( const AxialTree& axiom );
  const AxialTree& getAxiom( ) const ;

  /** Plot and interpret */
  void plot( AxialTree& workstring );
  inline void interpret( AxialTree& workstring)
  { interpret(workstring,__context.envturtle); }
  void interpret( AxialTree& workstring, PGL::Turtle& t );
  PGL(ScenePtr) sceneInterpretation( AxialTree& workstring );

  AxialTree homomorphism(AxialTree& workstring);

  /** Iterate */
  inline AxialTree iterate( )
  { return iterate(0,__max_derivation,__axiom); }
  inline AxialTree iterate( size_t nb_iter )
  { return iterate(0, nb_iter,__axiom); }

  AxialTree iterate( size_t starting_iter , 
                     size_t nb_iter , 
                     const AxialTree& workstring, 
                     bool previouslyinterpreted = false  );

  /** Animation */
  inline AxialTree animate()
  { return animate(__context.get_animation_timestep(),0,__max_derivation); }
  inline AxialTree animate(double dt)
  { return animate(dt,0,__max_derivation); }
  inline AxialTree animate(double dt, size_t order)
  { return animate(dt,0,order); }
  inline AxialTree animate(double dt, size_t beg, size_t nb_iter)
  { return animate(__axiom,dt,beg,nb_iter); }
  AxialTree animate(const AxialTree& workstring, double, size_t beg, size_t nb_iter);

  /** Record */
  inline void record(const std::string& prefix)
  { record(prefix,0,__max_derivation); }
  inline void record(const std::string& prefix, size_t nb_iter)
  { record(prefix,0,nb_iter); }
  void record(const std::string& prefix, size_t beg, size_t nb_iter);

  /** nb of iterations */
  inline size_t derivationLength() const { return __max_derivation; }
  inline void setDerivationLength(size_t v) { __max_derivation = v; }

  inline size_t decompositionMaxDepth() const { return __decomposition_max_depth; }
  inline void setDecompositionMaxDepth(size_t v) { __decomposition_max_depth = v; }

  inline size_t homomorphismMaxDepth() const { return __homomorphism_max_depth; }
  inline void setHomomorphismMaxDepth(size_t v) { __homomorphism_max_depth = v; }

  /** context */
  inline LsysContext * context() { return &__context; }
  /** make current or disable the context of self */
  inline void makeCurrent() { return __context.makeCurrent(); }
  inline bool isCurrent() const { return __context.isCurrent(); }
  inline void done() { return __context.done(); }

  /** direction of iteration */
  inline eDirection getDirection() const { return __context.getDirection(); }

  /** early return */
  void enableEarlyReturn(bool val) ;
  bool isEarlyReturnEnabled() ;
  size_t getLastIterationNb() { return __context.getIterationNb(); }

  /** test if self is actually iterating */
   bool isRunning() const;
protected:

  struct RuleGroup {
    RuleGroup();

    const RuleSet& getGroup(eRuleType) const;
    RuleSet& getGroup(eRuleType);
    bool hasQuery(eRuleType) const;

    RuleSet production;
    RuleSet decomposition;
    RuleSet homomorphism;
    bool __prodhasquery;
    bool __dechasquery;
    bool __homhasquery;
  };

  typedef std::vector<RuleGroup> RuleGroupList;

  LsysRule& __addRule( const std::string& rule, int type, size_t group,  int lineno = -1 );
  LsysRule& __addProdRule( const std::string& rule, size_t group,  int lineno = -1 );
  LsysRule& __addDecRule( const std::string& rule, size_t group,  int lineno = -1 );
  LsysRule& __addHomRule( const std::string& rule, size_t group,  int lineno = -1 );

  void __importPyFunctions();

 Lsystem(const Lsystem& lsys);
 Lsystem& operator=(const Lsystem& lsys);

 AxialTree __homomorphism(AxialTree& workstring);
 void __plot(AxialTree& workstring );
 void __interpret(AxialTree& workstring, PGL::Turtle& t);
 AxialTree __iterate( size_t starting_iter , 
                      size_t nb_iter , 
                      const AxialTree& workstring, 
                      bool previouslyinterpreted = false);

 AxialTree __step(AxialTree& workingstring,
				   const RulePtrSet& ruleset,
				   bool query,bool& matching,
                   eDirection direction);
 void __clear();

 AxialTree __stepWithMatching(AxialTree& workingstring,
				              const RulePtrSet& ruleset,
				              bool query,
                              StringMatching& matching);
 AxialTree __recursiveSteps(AxialTree& workingstring,
				            const RulePtrSet& ruleset, 
                            size_t maxdepth);

 void __recursiveInterpretation(AxialTree& workingstring,
				                const RulePtrSet& ruleset,
                                PGL::Turtle& turtle,
                                size_t maxdepth, bool withid = true);

 RulePtrSet __getRules(eRuleType type, size_t group, eDirection direction, bool * hasQuery = NULL);

  AxialTree __axiom;
  RuleGroupList __rules;

  size_t __max_derivation;
  size_t __decomposition_max_depth;
  size_t __homomorphism_max_depth;
  size_t __currentGroup;
  LocalContext __context;
  
  RuleGroup& __group(size_t group) ;
  const RuleGroup& __group(size_t group) const;


private:
#ifdef MULTI_THREADED_LSYSTEM
  void acquire() const;
  void release() const;
  class LsysAcquirer {
    public:
      const Lsystem * __lsys;
      LsysAcquirer(const Lsystem * lsys) ;
      ~LsysAcquirer() ;
  };
  friend class LsysAcquirer;

  struct LsysRessource {
      LsysRessource();
      QMutex mutex;
  };
  LsysRessource * __ressource;
#endif

  // For multithreaded appli, allow to set an early_return
  bool __early_return;
  QReadWriteLock __early_return_mutex;
};

/*---------------------------------------------------------------------------*/

LPY_END_NAMESPACE

#endif
