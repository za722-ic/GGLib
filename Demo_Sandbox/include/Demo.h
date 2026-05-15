#pragma once

#include "GGApp.h"
#include "AssetManagerWrapper.h"

#include "Text.h"

class Demo: public GGApp, KeyEventListener, MouseEventListener
{
private:
	std::shared_ptr<AssetManagerWrapper> demoAssetManager;

	Container* root; // TODO: maybe make a UI class that inherits Container, but is the only one that has calculateLayout. Since only root containers should have that?

	float frameTimesAcc;
	unsigned int framesCount;
	float avgFps;

	bool oldRenderer = false;

	std::string loremIpsum = " Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum tempus, ante at dignissim vulputate, diam ex sollicitudin mi, sit amet feugiat leo est vitae nulla. Quisque vestibulum eleifend velit aliquet aliquet. Nullam varius magna augue, eget venenatis lacus tincidunt et. Suspendisse imperdiet tempor odio, vel dapibus mauris tincidunt quis. Nunc id tristique ante, et interdum est. Aliquam erat volutpat. Praesent eget luctus sem. Etiam suscipit dolor quis fermentum luctus. Quisque hendrerit, massa auctor ultricies ullamcorper, nisi ipsum accumsan leo, at porttitor dolor tortor sit amet massa.\n\n";

	std::string loremIpsum2 = " Lorem ipsum dolor sit amet, consectetur adipiscing elit. Vestibulum tempus, ante at dignissim vulputate, diam ex sollicitudin mi, sit amet feugiat leo est vitae nulla. Quisque vestibulum eleifend velit aliquet aliquet. Nullam varius magna augue, eget venenatis lacus tincidunt et. Suspendisse imperdiet tempor odio, vel dapibus mauris tincidunt quis. Nunc id tristique ante, et interdum est. Aliquam erat volutpat. Praesent eget luctus sem. Etiam suscipit dolor quis fermentum luctus. Quisque hendrerit, massa auctor ultricies ullamcorper, nisi ipsum accumsan leo, at porttitor dolor tortor sit amet massa.\n\n"
		"Nunc auctor turpis eget fermentum vulputate.Ut blandit porta ante venenatis bibendum.Sed vel mollis eros.Nullam in faucibus erat.Aenean dictum ipsum nec urna sollicitudin cursus aliquam quis massa.Duis enim felis, laoreet eu gravida eu, mattis nec ligula.Etiam placerat pharetra turpis, ac efficitur massa scelerisque a.Morbi sit amet iaculis libero.Quisque mollis et dui et laoreet.Morbi quam nunc, aliquam vel consectetur sit amet, pellentesque ut elit.\n\n"
		"Nullam vel nisl id orci ornare suscipit ac nec erat.Nam varius lorem quis tortor lobortis mattis.Nullam condimentum faucibus egestas.Fusce molestie risus quis nibh ultricies, quis auctor massa tincidunt.Nunc aliquet mollis ornare.Proin finibus ligula est, sit amet feugiat est maximus eu.Ut euismod molestie tristique.Curabitur in nunc in erat iaculis sodales.Mauris ut nunc orci.\n\n"
		"Interdum et malesuada fames ac ante ipsum primis in faucibus.Ut vehicula tortor sapien, id accumsan magna malesuada non.In sit amet viverra odio, non tristique elit.In cursus efficitur justo, vel imperdiet massa fringilla ut.Suspendisse non erat sollicitudin, vulputate tortor ac, rutrum eros.Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos.Maecenas in elit suscipit diam posuere pulvinar.Donec magna leo, sollicitudin eget sagittis nec, blandit eu est.Nullam ut nulla velit.Sed luctus faucibus suscipit.Vivamus non metus ultrices, dictum odio eu, semper justo.\n\n"
		"Vestibulum porttitor sem sodales purus mattis finibus.Nullam pharetra volutpat metus vitae consectetur.Nam dui odio, malesuada et sem eu, ultricies ultricies turpis.Aliquam vitae varius ligula, ut laoreet tortor.Maecenas sit amet velit convallis, finibus odio ac, rutrum orci.Quisque malesuada suscipit luctus.Quisque mi neque, convallis vitae nulla vitae, tempor commodo eros.Morbi gravida sapien est, ut mattis metus faucibus non.Fusce vitae varius sem, sit amet hendrerit ex.Duis ut ornare purus, id accumsan ligula.Sed aliquet ac nibh nec faucibus.\n\n"
		"Sed scelerisque, enim semper eleifend consequat, magna nisi facilisis purus, sed sollicitudin risus quam quis dolor.Nunc dapibus urna odio, ut eleifend velit egestas sit amet.Vestibulum auctor erat id nulla varius tempus.Mauris lorem massa, iaculis et facilisis et, imperdiet ut arcu.Quisque ut augue efficitur, ornare neque nec, mattis libero.Sed in ex ac leo tincidunt tincidunt nec blandit tellus.Cras consequat, tortor nec cursus tincidunt, tortor est maximus sapien, id volutpat massa leo sed ipsum.Suspendisse vel turpis eget elit vestibulum auctor.\n\n"
		"Curabitur finibus sapien eget nisl aliquam pellentesque id eu velit.Etiam tempor urna nec ipsum ornare dapibus.Vestibulum tincidunt justo ex, eu semper massa tincidunt quis.Interdum et malesuada fames ac ante ipsum primis in faucibus.Praesent laoreet felis lectus, eu tempus felis lobortis nec.In pretium, magna sit amet placerat ultrices, quam tortor consectetur nisi, ac tincidunt ligula ipsum at tortor.Cras gravida dignissim lacus at iaculis.Curabitur ultrices hendrerit scelerisque.Praesent finibus arcu a arcu elementum pharetra.Nulla venenatis condimentum neque, eu vestibulum lectus efficitur eu.Etiam et nibh sit amet arcu vulputate gravida.Vestibulum dapibus dui at ornare facilisis.Curabitur vitae auctor nibh, id posuere sem.Curabitur efficitur arcu bibendum, condimentum tellus et, posuere est.\n\n"
		"Donec interdum iaculis justo quis tempus.Maecenas iaculis enim sit amet augue rhoncus, non rhoncus arcu pellentesque.Maecenas est tellus, congue quis ultricies luctus, fermentum a ipsum.Nunc vitae tortor facilisis, fringilla massa et, aliquam dolor.Donec consequat pretium interdum.Aenean non gravida leo.Ut faucibus justo vel ligula luctus consequat.Vivamus imperdiet ante vel semper posuere.\n\n"
		"Nullam auctor erat nec accumsan porttitor.Curabitur rutrum placerat libero et aliquet.Suspendisse mattis tellus diam, vel tincidunt nulla rutrum sit amet.Proin mollis, ante ut vestibulum efficitur, mi sapien luctus est, at aliquet diam nisl et arcu.Maecenas leo mauris, mollis rhoncus enim non, tristique egestas dolor.Pellentesque dignissim quam dolor, eget malesuada magna accumsan et.Phasellus sagittis porttitor velit, dictum gravida enim hendrerit eget.Donec elementum, mauris a tincidunt dapibus, augue neque viverra mauris, congue tincidunt libero arcu ac nibh.Suspendisse laoreet nibh quis cursus tristique.Morbi sed iaculis justo, sed ultricies ex.Curabitur enim magna, interdum ut nunc non, dictum commodo augue.Cras vel orci euismod, aliquet est vitae, congue metus.Phasellus fringilla sollicitudin libero, ac lacinia justo lacinia ac.\n\n"
		"Sed aliquam, quam sed sollicitudin cursus, urna urna scelerisque ligula, nec fermentum neque metus non odio.Proin tincidunt luctus augue, vel sagittis mauris.Integer ut lectus in diam tristique feugiat sit amet sed mauris.Suspendisse potenti.Proin justo erat, tincidunt quis metus at, feugiat lacinia elit.Vivamus congue libero quis cursus laoreet.Aliquam auctor erat tellus, nec euismod augue maximus in.Aliquam egestas elit a risus posuere tincidunt.Sed ac ullamcorper velit.Suspendisse pretium nisi at mi eleifend egestas non in augue.\n\n"
		"Fusce semper ullamcorper turpis viverra condimentum.Mauris ultrices, metus eget vestibulum tempus, metus diam auctor tortor, vulputate porttitor lectus velit sed libero.Suspendisse potenti.Etiam nec fringilla nulla.Vivamus vulputate bibendum iaculis.Etiam ac molestie ligula, eu efficitur massa.Nulla vel massa at ipsum pulvinar facilisis sit amet vitae tellus.Pellentesque ac mattis metus.Vestibulum faucibus tortor eget nunc fringilla, a eleifend nunc dapibus.\n\n"
		"In ac bibendum felis.Sed pharetra lacus in massa tristique viverra.Aenean pellentesque pellentesque nisl, sed convallis ex congue non.Ut ultrices neque nec fermentum vehicula.Nullam mauris libero, ullamcorper vel faucibus in, pellentesque non massa.Quisque placerat venenatis metus in molestie.Cras viverra rutrum dui at dignissim.\n\n";

public:
	Demo() : demoAssetManager(std::make_shared<AssetManagerWrapper>(&assetManager))
	{}

private:

	bool onInit() override;
	void onLoop() override;
	void onQuit() override;

	void onKeyEvent(KeyEventType keyEventType, SDL_Keycode key) override;
	void onMouseEvent(MouseEventType mouseEventType, int mouseX, int mouseY) override;

	void defineElements();
};
