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
        virtual Currency getNetPrice() {
            return netPrice;
        }
        virtual void setNetPrice(Currency netPrice_) {
            netPrice = netPrice_;
        }
        // declares an interface for accessing and managing its child components.
        //   virtual void add(std::shared_ptr<Menu>) = 0;
        //   virtual void remove(std::shared_ptr<Menu>) = 0;
        virtual ~Menu() = default;
    protected:
        Menu() :name(""), netPrice(0) {}
        Menu(const std::string& name_) :name(name_), netPrice(0) {}
    private:
        std::string name;
        Currency netPrice;
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
        virtual void setDelay(uint8_t& delay_){
            delay = delay_;
        }
    protected:
        MenuHub() :Menu() {}
        MenuHub(const std::string& name_) :Menu() {
            setName(name_);
        }
    private:
        // stores child components.
        std::list<Menu*> Menu;
        uint8_t delay;
};


// represents leaf objects in the composition.
class MenuItem : public Menu { // Leaf
    public:
        MenuItem(const std::string& name_) {
            setName(name_);
        }
        void setTime(null time_){
            time = time_;
        }
        void setDelay(uint8_t delay_){
            delay = delay_;
        }
        void setInterval(bool interval_){
            interval = interval_;
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
};



//////////////////////////////////////////////////////////////////////////////////////////
int main() {

}