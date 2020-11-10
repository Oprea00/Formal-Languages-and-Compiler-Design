class FiniteAutomata:
    def __init__(self, fileName):
        self.__states = []
        self.__alphabet = []
        self.__transitions = {}
        self.__initialState = None
        self.__finalStates = []
        self.readFA_FromFile(fileName) 
        self.__isDFA = True
     
         
    def parseLine(self,line):
        return line.strip().split(" ")     
         
    def readFA_FromFile(self, fileName):   
        with open(fileName, "r") as file:
            
            states = file.readline()
            self.__states = self.parseLine(states)
            
            alphabet = file.readline()
            self.__alphabet = self.parseLine(alphabet)
            
            transitionsLine = file.readline()
            transitions = transitionsLine.strip().split(";")
            for transition in transitions:
                currentTransition = self.parseLine(transition)
                if (currentTransition[0], currentTransition[1]) in self.__transitions.keys():
                    self.__transitions[(currentTransition[0], currentTransition[1])].append(currentTransition[2])
                    self.__isDFA = False
                else:    
                    self.__transitions[(currentTransition[0], currentTransition[1])]=currentTransition[2]
                
            initialState = file.readline()
            self.__initialState = initialState.strip()
            
            finalStates = file.readline()
            self.__finalStates = self.parseLine(finalStates)    
      
            
    def displayStates(self):
        print("The set of states (Q): " , self.__states)
        
    def displayAlphabet(self):
        print("The alphabet (E): " , self.__alphabet)
        
    def displayTransitions(self):
        print("Transitions: ")
        for transition in self.__transitions.keys():
            print("({0}, {1}) --> {2}".format(transition[0], transition[1], self.__transitions[transition]))
            
    def displayInitialState(self):
        print("Initial state (q0): " , self.__initialState)
        
    def displayFinalStates(self):        
        print("Final States (F): " , self.__finalStates)
    
    def displayMenu(self): 
        print("\n")
        print("Choose an option for printing: ")
        print("    1. The set of states")           
        print("    2. The alphabet")
        print("    3. Transitions")
        print("    4. Initial state")
        print("    5. Final states")
        print("    6. Verify if a sequence is accepted by the FA")
        print("    0. Exit")
        print("\n")
        
    def verifySequence(self, dfa): #aab accepted, aba not accepted
        if not self.__isDFA:
            print("Not DFA")
            return
        
        currentState = self.__initialState
        while len(dfa) > 0 and currentState is not None:
            transition = (currentState, dfa[0])
            #print(transition)
            if transition in self.__transitions.keys():
                currentState = self.__transitions[transition]
                #print(currentState)
                dfa = dfa[1:]
            else:
                return False
        return currentState is not None and currentState in self.__finalStates     
        
        
def run():
    FA =FiniteAutomata("FA.in.txt") 
    while True :
        FA.displayMenu()
        option = int(input("Choose option: "))
            
        if option == 1:
            FA.displayStates()
        elif option == 2:
            FA.displayAlphabet()
        elif option == 3:
            FA.displayTransitions()
        elif option == 4:
            FA.displayInitialState()
        elif option == 5:
            FA.displayFinalStates()  
        elif option == 6:
            sequence = input("Enter sequence here: ")    
            result = FA.verifySequence(sequence)
            if result:
                print("Sequence accepted!")
            else:
                print("Sequence NOT accepted")    
        elif option == 0: 
            return    
        else:
            print("Wrong command!")  
                                
run()                                