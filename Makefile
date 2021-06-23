.PHONY: serve
serve:
	$(MAKE) serve-backend & $(MAKE) serve-frontend

.PHONY: serve-backend
serve-backend: src/backend/waze-server rj_graph_database.json
	./$<

.PHONY: serve-frontend
serve-frontend:
	$(MAKE) -C src/frontend/ serve

.PHONY: test-shortest-path
test-shortest-path:
	$(MAKE) -C src/backend/ test

.PHONY: src/backend/waze-server
src/backend/waze-server:
	$(MAKE) -C src/backend/ waze-server

rj_graph_database.json: src/create_data/create_graph_database.py
	python3 $<
	sed -i 's/ //g' $@
