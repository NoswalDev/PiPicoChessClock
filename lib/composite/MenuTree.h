#include <ctime>

// declares the interface for objects in the composition.
class Menu { // Component
    public:
        // implements default behavior for the interface common to all classes, as appropriate.
        virtual const std::string& getName() {
            return name;
        }
        virtual void setName(const std::string& name_) {
            name = name_;
        }
        virtual void std::string& getDspName() {
            return dspName;
        }
        virtual void setDspName(std::string& dspName_) {
            dspName = dspName_;
        }
        // declares an interface for accessing and managing its child components.
        //   virtual void add(std::shared_ptr<Menu>) = 0;
        //   virtual void remove(std::shared_ptr<Menu>) = 0;

        virtual ~Menu() = default;
    protected:
        Menu() :name(""), dspName("") {}
        Menu(const std::string& name_) :name(name_), dspName(dspName_) {}
    private:
        std::string name;
        std::string dspName;
        
};

// defines behavior for components having children.
class MenuHub : public Menu { // Composite
    public:
        // implements child-related operations in the Component interface.
        //   virtual Currency getNetPrice() override {
        //     Currency total = Menu::getNetPrice();
        //     for (const auto& i:Menu) {
        //       total += i->getNetPrice();
        //     }
        //     return total;
        //   }
        virtual void add(std::shared_ptr<Menu> Menu_) { //override at the end
            Menu.push_back(Menu_.get());
        }
        virtual void remove(std::shared_ptr<Menu> Menu_) { //override at the end
            Menu.remove(Menu_.get());
        }
    protected:
        MenuHub() :Menu() {}
        MenuHub(const std::string& name_) :Menu() {
            setName(name_);
        }
    private:
        // stores child components.
        std::list<Menu*> Menu;
        
};

// represents leaf objects in the composition.
class MenuItem : public Menu { // Leaf
    public:
        MenuItem(const std::string& name_) {
            setName(name_);
        }
        void setTime(null time_){ //set time in seconds
            time = time_;
        }
        void setDelay(uint8_t delay_){
            delay = delay_;
        }
        void keepDelay(bool interval_){ //bool to indicate if delay is 'delay' or 'interval'
            interval = interval_;
        }
        void delayBefore(bool delayBefore_){
            delayBefore = delayBefore_
        }
    // A leaf has no children.
    //   void add(std::shared_ptr<Menu>) override {
    //     throw std::runtime_error("MenuItem::add");
    //   }
    //   void remove(std::shared_ptr<Menu>) override {
    //     throw std::runtime_error("MenuItem::remove");
    //   }
    protected:
    private:
        uint8_t delay;
        null time;
        bool interval;
        bool delayBefore;
};

class Outline : public Menu { //overseer. provides gfx info to draw overview.
    public:
        Outline(Menu root_){
            root = root_;
        }
    protected:
    private:
        Menu root;
};





//////////////////////////////////////////////////////////////////////////////////////////
int main() {

};
/* outline
item cursor;
cursor.visiting(node.Menu[0]); //cursor points to first menuitem in the menuhub
cursor.run(); //cursor activates what it's visiting, which could be a mode or param setting
cursor.back(); //goes back up to previous hub

visitor cursor{
    virtual void openNode
    virtual void 
}

need overseer object to pull tree info for graphics generation
initialize tree


*/